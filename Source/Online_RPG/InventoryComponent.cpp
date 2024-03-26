// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(): InventorySlotCapacity(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UInventoryComponent::AddMoney(const uint64 AmountToAddMoney)
{
	CurrentMoney += AmountToAddMoney;
	OnInventoryUpdated.Broadcast();
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (!ItemIn) return nullptr;

	if (InventoryContents.Contains(ItemIn))
	{
		return ItemIn;
	}

	return nullptr;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UInventoryComponent::AddStackableItem(UItemBase* InputItem, int32 AddAmount)
{
	if (AddAmount <=0) return 0;

	int32 AmountToDistribute = AddAmount;
	
	UItemBase* CurrentStackingItem = FindNextPartial(InputItem);

	
	while (CurrentStackingItem)
	{
		if (AmountToDistribute == 0) break;
		
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(CurrentStackingItem,AmountToDistribute);
		
		CurrentStackingItem->SetQuantity(CurrentStackingItem->BaseItemQuantity + AmountToMakeFullStack);
		
		AmountToDistribute -= AmountToMakeFullStack;

		CurrentStackingItem = FindNextPartial(InputItem);
	}
	
	// 슬롯이 남아있고, 남은 양이 0개 초과일 경우
	while (InventoryContents.Num()+1 <= InventorySlotCapacity )
	{
		if (AmountToDistribute == 0) break;
		
		const int32 AmountToAdd = FMath::Clamp(AmountToDistribute,0,InputItem->BaseItemNumericData.MaxStackSize);
		AddNewItem(InputItem->CreateCopy(),AmountToAdd);
		AmountToDistribute -= AmountToAdd;
	}

	InputItem->SetQuantity(AmountToDistribute);
	OnInventoryUpdated.Broadcast();
	 return AddAmount - AmountToDistribute;
}

FItemAddResultData UInventoryComponent::AddNonStackableItem(UItemBase* InputItem)
{
	if (InventoryContents.Num() + 1 > InventorySlotCapacity)
	{
		return FItemAddResultData::AddNone(FText::FromString("인벤토리가 가득 찼습니다."));
	}

	AddNewItem(InputItem,1);
	
	return FItemAddResultData::AddAll(1,FText::Format(
	FText::FromString("Successfully added {1} to the inventory"),
	InputItem->BaseItemTextData.NameText));
	
}

// AddNewItem, 납득이 잘 안됨
void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	if (AmountToAdd == 0) return;
	
	UItemBase* NewItem;

	// 각각의 아이템은 각각의 bIsPickUp, bIsCopy를 가짐
	if (Item->bIsCopy || Item->bIsPickUp)
	{
		// Copy 이거나, 픽업아이템일때
		// Item이 newItem이 됨
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// 새 객체 NewItem을 생성함
		// bIsCopy = true로 생성함
		// 아이템을 드래그하거나, 아이템을 나눌 때 사용
		// 인벤토리 내에서 상호작용을 할 때 사용
		NewItem = Item->CreateCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::RemoveItemFromList(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* RemoveItem, const int32 AmountToRemove) const
{
	const int32 ActualAmountToRemove = FMath::Min(AmountToRemove,RemoveItem->BaseItemQuantity);
	RemoveItem->SetQuantity(RemoveItem->BaseItemQuantity-ActualAmountToRemove);

	OnInventoryUpdated.Broadcast();
	return ActualAmountToRemove;
}

UItemBase* UInventoryComponent::FindNextPartial(UItemBase* ItemIn) const
{
	const TArray<TObjectPtr<UItemBase>>::ElementType* Result;

	Result = InventoryContents.FindByPredicate
	([&ItemIn](const UItemBase* InventoryItem){
	return InventoryItem->BaseItemID == ItemIn->BaseItemID && !InventoryItem->IsFullItemStack();
});
	if (Result)
	{
		return *Result;
	}

	return nullptr;
}

int32 UInventoryComponent::CalculateNumberForFullStack(const UItemBase* StackableItem, const int32 AddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->BaseItemNumericData.MaxStackSize - StackableItem->BaseItemQuantity;

	//UE_LOG(LogTemp,Warning,TEXT("item: %d, addamount = %d, FullStack = %d" ),StackableItem->BaseItemQuantity,AddAmount,AddAmountToMakeFullStack)
	
	return FMath::Min(AddAmount,AddAmountToMakeFullStack);
}



FItemAddResultData UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (!GetOwner()) return FItemAddResultData::AddNone(FText::FromString("오류발생"));

	const int32 InitialRequestedAddAmount = InputItem->BaseItemQuantity;

	if (!InputItem->BaseItemNumericData.bIsStackable)
	{
		return AddNonStackableItem(InputItem);
	}

	if (InputItem->BaseItemType == EItemType::Money)
	{
		AddMoney(InitialRequestedAddAmount);
		
		
		return FItemAddResultData::AddAll(InitialRequestedAddAmount,
			FText::Format(FText::FromString("{0}원을 획득하였습니다"),InitialRequestedAddAmount));
	}
	
	const int32 StackableAmountAdded = AddStackableItem(InputItem,InitialRequestedAddAmount);

	if (StackableAmountAdded == InitialRequestedAddAmount)
	{
		return FItemAddResultData::AddAll
		(InitialRequestedAddAmount,FText::Format(FText::FromString("{0} {1}개를 획득하였습니다"),
			InputItem->BaseItemTextData.NameText,
			StackableAmountAdded));
	}

	if (StackableAmountAdded < InitialRequestedAddAmount)
	{
		return FItemAddResultData::AddPartial
		(StackableAmountAdded,FText::Format(FText::FromString("{0} {1}/{2}개를 획득하였습니다."),
			InputItem->BaseItemTextData.NameText,
			StackableAmountAdded,
			InitialRequestedAddAmount
		));
	}

	if (StackableAmountAdded <= 0)
	{
		return FItemAddResultData::AddNone(FText::FromString("인벤토리가 가득 찼습니다"));
	}

	return FItemAddResultData::AddNone(FText::FromString("에러발생"));
}



