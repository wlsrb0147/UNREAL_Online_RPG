// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"

#include "InventoryComponent.h"
#include "ItemBase.h"
#include "ItemC.h"

// Sets default values
APickUpItem::APickUpItem(int32 InitialQuantity)
{
	bIsConstructing = true;
	// 드랍템
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InstanceMesh = CreateDefaultSubobject<UStaticMeshComponent>("CurrentMesh");
	InstanceMesh->SetSimulatePhysics(true);
	SetRootComponent(InstanceMesh);

    if (InitialQuantity <=0) InstanceItemQuantity = 1;
    else InstanceItemQuantity = InitialQuantity;
	
	InteractionData.InteractionType = EInteractionType::PickUp;
	bReplicates = true;

	
	
}

APickUpItem::APickUpItem()
{
	bIsConstructing = true;
	// 엔피씨
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InstanceMesh = CreateDefaultSubobject<UStaticMeshComponent>("CurrentMesh");
	InstanceMesh->SetSimulatePhysics(true);
	SetRootComponent(InstanceMesh);
	bReplicates = true;
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	InitializeItem(UItemBase::StaticClass(),InstanceItemQuantity);
}

void APickUpItem::InitializeItem(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	UE_LOG(LogTemp,Warning,TEXT("비긴 이니셜1"))
	if (InstanceItemDataTable && !InstanceItemID.IsNone())
	{
		const FItemData* ItemData = InstanceItemDataTable->FindRow<FItemData>(InstanceItemID,TEXT("Error"));
		
		InstanceItemData = NewObject<UItemBase>(this,BaseClass);
		
		InstanceItemData->BaseItemID = ItemData->ItemID;
		InstanceItemData->BaseItemType = ItemData->ItemType;
		InstanceItemData->BaseItemNumericData = ItemData->ItemNumericData;
		InstanceItemData->BaseItemTextData = ItemData->ItemTextData;
		InstanceItemData->BaseItemAssetData = ItemData->ItemAssetData;
		InstanceItemData->BaseItemStatistics = ItemData->ItemStatistics;
		
		if (InQuantity <=0) InstanceItemData->SetQuantity(1);
		else InstanceItemData->BaseItemQuantity = InQuantity;

		if (InstanceItemData->BaseItemNumericData.bIsStackable && InstanceItemData->BaseItemNumericData.MaxStackSize < 2 )
		{
			InstanceItemData->BaseItemNumericData.MaxStackSize = 2;
		}

		InstanceMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);
		UpdateItemInteractionData();
		
	}

	bIsConstructing = false;
}

void APickUpItem::UpdateItemInteractionData()
{
	// InteractionData는 Interface에 있는 정보
	// InstanceItemInteractData는 PickUpItem에 있는 정보
	// 정보 탐색은 interface에서 하고있음
	InstanceItemInteractData.Name = InstanceItemData->BaseItemTextData.NameText;
	InstanceItemInteractData.Quantity = InstanceItemData->BaseItemQuantity;
	InteractionData = InstanceItemInteractData;
}

void APickUpItem::ServerDestroyActor_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("실행됨!!!!!!777"));
	Destroy();
}

void APickUpItem::BeginFocus()
{
	IItemInteractionInterface::BeginFocus();
}

void APickUpItem::EndFocus()
{
	IItemInteractionInterface::EndFocus();
}

void APickUpItem::Interact(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter) PickUpItem(PlayerCharacter);
}

void APickUpItem::PickUpItem(const APlayerCharacter* Taker)
{
	if (IsPendingKillPending()) return;

	if (!InstanceItemData) return;
	// 아이템 액터의 소유자를 설정
	
	SetOwner(Taker->GetController());

	if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
	{
		const FItemAddResultData AddResult = PlayerInventory->HandleAddItem(InstanceItemData);

		switch (AddResult.AddResult) {
		case EItemAddOperationResult::IAR_NoItemAdded:
			break;
		case EItemAddOperationResult::IAR_PartialAmountItemAdded:
			UpdateItemInteractionData();
			Taker->UpdateInteractionWidget();
			break;
		case EItemAddOperationResult::IAR_AllItemAdded:
			//Destroy();
			ServerDestroyActor();
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Default실행"));
			
		}
	}
}

void APickUpItem::InitializeDropItem(UItemBase* ItemToDrop, const int32 Quantity)
{
	InitializeItem(UItemBase::StaticClass(),Quantity);
	
	InstanceItemData = ItemToDrop;
	InstanceItemData->SetQuantity(Quantity);
	InstanceItemData->OwningInventory = nullptr;
	InstanceMesh->SetStaticMesh(ItemToDrop->BaseItemAssetData.Mesh);

	UpdateItemInteractionData();
}

void APickUpItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property?
		PropertyChangedEvent.Property->GetFName() : NAME_None;
	
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickUpItem,InstanceItemID))
	{
		if (InstanceItemDataTable)
		{
			const FItemData* ChangeItemData =
				InstanceItemDataTable->FindRow<FItemData>(InstanceItemID,"Error");
			if (ChangeItemData)
			{
				InstanceMesh->SetStaticMesh(ChangeItemData->ItemAssetData.Mesh);
			}
		}
	}
}

