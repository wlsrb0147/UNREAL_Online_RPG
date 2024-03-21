// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"

#include "AITestsCommon.h"
#include "ItemBase.h"
#include "ItemStruct.h"


void ItemManager::Initialize(UDataTable* InItemDataTable,UWorld* World)
{
	
	if (!ItemDataTable)
	{
		ItemDataTable = InItemDataTable;
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Else 실행"))
	}

	if (!CurrentWorld)
	{
		CurrentWorld = World;
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("월드 Else 실행"))
	}
}

UItemBase* ItemManager::MakeItemBaseByKey(UObject* Outer, const FName Key, const int32 Quantity, const FString& Context) const
{
	return FindAndMakeItemBase(Outer,Key,Quantity,Context);
}

UItemBase* ItemManager::MakeItemBaseByKey(UObject* Outer, const FString& Key, const int32 Quantity, const FString& Context) const
{
	return FindAndMakeItemBase(Outer,FName(Key),Quantity,Context);
}

UItemBase* ItemManager::FindAndMakeItemBase(UObject* Outer, const FName Key, const int32 Quantity, const FString& Context) const
{
	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(FName(Key), *Context);
	MakeItemBase(Outer,ItemData,Quantity);
	
	return MakeItemBase(Outer,ItemData,Quantity);
}

UItemBase* ItemManager::MakeItemBase(UObject* Outer, const FItemData* ItemData, const int32 Quantity)
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

APickUpItem* ItemManager::SpawnItem(AActor* Outer, UItemBase* ItemBase, const FTransform& Transform, const int32 Quantity) const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Outer;
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	APickUpItem* DropItem = CurrentWorld->SpawnActor<APickUpItem>(APickUpItem::StaticClass(), Transform, SpawnParameters);
	DropItem->InitializeDropItem(ItemBase, Quantity);
	return DropItem;
}

