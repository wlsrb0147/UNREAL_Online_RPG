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
	return 1;
}

FItemAddResult UInventoryComponent::AddNonStackableItem(UItemBase* InputItem)
{
	if (InventoryContents.Num() + 1 > InventorySlotCapacity)
	{
		return FItemAddResult::AddNone(FText::FromString("인벤토리가 가득 찼습니다."));
	}

	AddNewItem(InputItem,1);
	
	return FItemAddResult::AddAll(1,FText::Format(
	FText::FromString("Successfully added {1} to the inventory"),
	InputItem->BaseItemTextData.NameText));
	
}

// AddNewItem, 납득이 잘 안됨
void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
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

void UInventoryComponent::RemoveSingleItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}


FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->BaseItemQuantity;

		if (!InputItem->BaseItemNumericData.bIsStackable)
		{
			return AddNonStackableItem(InputItem);
		}

		const int32 StackableAmountAdded = AddStackableItem(InputItem,InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddAll
			(StackableAmountAdded,FText::Format(FText::FromString("{0} {1}개를 획득하였습니다"),
				InputItem->BaseItemTextData.NameText,
				StackableAmountAdded));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount)
		{
			return FItemAddResult::AddPartial
			(StackableAmountAdded,FText::Format(FText::FromString("{0} {1}/{2}개를 획득하였습니다."),
				InputItem->BaseItemTextData.NameText,
				StackableAmountAdded,
				InitialRequestedAddAmount
			));
		}

		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddNone(FText::FromString("인벤토리가 가득 찼습니다"));
		}
		
	}
	return FItemAddResult::AddNone(FText::FromString("오류발생"));
}



