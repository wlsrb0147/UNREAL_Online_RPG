// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "PickUpItem.h"

struct FItemData;
class UItemBase;
/**
 * 
 */
class ONLINE_RPG_API ItemManager final
{
public:
	static ItemManager& Get()
	{
		static ItemManager Instance;
		return Instance;
	}
	
	void Initialize(UDataTable* InItemDataTable,UWorld* World);
	
    UItemBase* MakeItemBaseByKey(UObject* Outer,const FName Key, const int32 Quantity, const FString& Context = TEXT("")) const;
    UItemBase* MakeItemBaseByKey(UObject* Outer,const FString& Key, const int32 Quantity, const FString& Context = TEXT("")) const;
	UItemBase* FindAndMakeItemBase(UObject* Outer,const FName Key, const int32 Quantity, const FString& Context = TEXT("")) const;
	static UItemBase* MakeItemBase(UObject* Outer, const FItemData* ItemData, const int32 Quantity);
	
	APickUpItem* SpawnItem(AActor* Outer,UItemBase* ItemBase, const FTransform& Transform, int32 Quantity) const;

	UDataTable* ItemDataTable = nullptr;
	UWorld* CurrentWorld = nullptr;
	
private:
	
	ItemManager() = default;
	~ItemManager() = default;
	
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;
	
};
