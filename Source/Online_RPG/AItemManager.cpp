// Fill out your copyright notice in the Description page of Project Settings.


#include "AItemManager.h"

#include "AITestsCommon.h"
#include "ItemBase.h"
#include "ItemStruct.h"
#include "PickUpItem.h"


class APickUpItem;

void AItemManager::BeginPlay()
{
	Super::BeginPlay();
	CurrentWorld = GetWorld();
}


UItemBase* AItemManager::MakeItemBaseByKey(UObject* Outer, const FName Key, const int32 Quantity, const FString& Context) const
{
	return FindAndMakeItemBase(Outer,Key,Quantity,Context);
}

UItemBase* AItemManager::MakeItemBaseByKey(UObject* Outer, const FString& Key, const int32 Quantity, const FString& Context) const
{
	return FindAndMakeItemBase(Outer,FName(Key),Quantity,Context);
}

UItemBase* AItemManager::MakeItemBaseByKey(UObject* Outer, const int32 Key, const int32 Quantity,
	const FString& Context) const
{
	return FindAndMakeItemBase(Outer,FName(FString::FromInt(Key)),Quantity,Context);
}

UItemBase* AItemManager::FindAndMakeItemBase(UObject* Outer, const FName Key, const int32 Quantity, const FString& Context) const
{
	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(FName(Key), *Context);
	MakeItemBase(Outer,ItemData,Quantity);
	
	return MakeItemBase(Outer,ItemData,Quantity);
}

UItemBase* AItemManager::MakeItemBase(UObject* Outer, const FItemData* ItemData, const int32 Quantity)
{
	UItemBase* ItemCopy = NewObject<UItemBase>(Outer,UItemBase::StaticClass());
	
	ItemCopy->BaseItemID = ItemData->ItemID;
	ItemCopy->BaseItemType = ItemData->ItemType;
	ItemCopy->BaseItemNumericData = ItemData->ItemNumericData;
	ItemCopy->BaseItemTextData = ItemData->ItemTextData;
	ItemCopy->BaseItemAssetData = ItemData->ItemAssetData;
	ItemCopy->BaseItemStatistics = ItemData->ItemStatistics;
	ItemCopy->SetQuantity(Quantity);

	if (ItemCopy->BaseItemNumericData.bIsStackable && ItemCopy->BaseItemNumericData.MaxStackSize < 2 )
	{
		ItemCopy->BaseItemNumericData.MaxStackSize = 2;
	}
	
	return ItemCopy;
}

void AItemManager::SpawnItem(AActor* Outer, UItemBase* ItemBase,const FTransform& Transform, const int32 Quantity) const
{
	FActorSpawnParameters SpawnParameters;
	//SpawnParameters.Owner = Outer;
	SpawnParameters.Owner = GetWorld()->GetFirstPlayerController()->GetPawn();  // ���� ����
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	APickUpItem* DropItem = CurrentWorld->SpawnActor<APickUpItem>(APickUpItem::StaticClass(), Transform, SpawnParameters);
	DropItem->SetOwner(GetWorld()->GetFirstPlayerController());
	DropItem->ReplicatedOwner = GetWorld()->GetFirstPlayerController()->GetPawn();
	//DropItem->a++;
	
	int32 Key = FCString::Atoi(*ItemBase->BaseItemID.ToString());
	DropItem->InitializeDropItem(Key, Quantity);
	
}

