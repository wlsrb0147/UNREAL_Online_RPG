// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"

#include "ItemBase.h"

// Sets default values
APickUpItem::APickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InstanceMesh = CreateDefaultSubobject<UStaticMeshComponent>("CurrentMesh");
	InstanceMesh->SetSimulatePhysics(true);
	SetRootComponent(InstanceMesh);
}

void APickUpItem::InitializeItem(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (InstanceItemDataTable && !InstanceItemID.IsNone())
	{
		const FItemData* ItemData = InstanceItemDataTable->FindRow<FItemData>(InstanceItemID,TEXT("Error"));

		InstanceItemData = NewObject<UItemBase>(this,BaseClass);
		
		InstanceItemData->BaseItemID = ItemData->ItemID;
		InstanceItemData->BaseItemType = ItemData->ItemType;
		InstanceItemData->BaseItemNumericData = ItemData->ItemNumericData;
		InstanceItemData->BaseItemTextData = ItemData->ItemTextData;
		InstanceItemData->BaseItemAssetData = ItemData->ItemAssetData;

		InQuantity <=0 ? InstanceItemData->BaseItemQuantity =1 : InstanceItemData->BaseItemQuantity =InQuantity ;

		InstanceMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);
	}
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	InitializeItem(UItemBase::StaticClass(),InstanceItemQuantity);
}

void APickUpItem::BeginFocus()
{
	IItemInteractionInterface::BeginFocus();
}

void APickUpItem::EndFocus()
{
	IItemInteractionInterface::EndFocus();
}

void APickUpItem::BeginInteract()
{
	IItemInteractionInterface::BeginInteract();
}

void APickUpItem::Interact(APlayerCharacter* PlayerCharacter)
{
	IItemInteractionInterface::Interact(PlayerCharacter);
}

void APickUpItem::EndInteract()
{
	IItemInteractionInterface::EndInteract();
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

