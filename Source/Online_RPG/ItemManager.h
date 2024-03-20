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
	
	void Initialize(UDataTable* InItemDataTable);
	
    UItemBase* MakeItemBaseByKey(UObject* Outer,const FName Key, const int32 Quantity, const FString& Context = TEXT("")) const;
    UItemBase* MakeItemBaseByKey(UObject* Outer,const FString& Key, const int32 Quantity, const FString& Context = TEXT("")) const;
	UItemBase* FindAndMakeItemBase(UObject* Outer,const FName Key, const int32 Quantity, const FString& Context = TEXT("")) const;
	static UItemBase* MakeItemBase(UObject* Outer, const FItemData* ItemData, const int32 Quantity);
	
	static void SpawnItem(UObject* Outer,UItemBase* ItemBase,const FVector& Location, const FRotator& Rotation);

	UDataTable* ItemDataTable = nullptr;
	
private:
	
	ItemManager() = default;
	~ItemManager() = default;
	
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;
	
};
