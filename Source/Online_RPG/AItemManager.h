// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "AItemManager.generated.h"

struct FItemData;
class UItemBase;
/**
 * 
 */
UCLASS()
class ONLINE_RPG_API AItemManager : public AActor
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	/*static ItemManager& Get()
	{
		static ItemManager Instance;
		return Instance;
	}*/
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APickUpItem> Pickup_Class;

    UItemBase* MakeItemBaseByKey(UObject* Outer,const FName Key, const int32 Quantity, const FString& Context = TEXT("")) const;
    UItemBase* MakeItemBaseByKey(UObject* Outer,const FString& Key, const int32 Quantity, const FString& Context = TEXT("")) const;
	UItemBase* MakeItemBaseByKey(UObject* Outer,const int32 Key, const int32 Quantity, const FString& Context = TEXT("")) const;
	
	void SpawnItem(AActor* Outer,UItemBase* ItemBase,const FTransform& Transform, int32 Quantity) const;

	UPROPERTY(EditAnywhere)
	UDataTable* ItemDataTable = nullptr;

	UPROPERTY()
	UWorld* CurrentWorld = nullptr;
	
private:
	UItemBase* FindAndMakeItemBase(UObject* Outer,const FName Key, const int32 Quantity, const FString& Context = TEXT("")) const;
	static UItemBase* MakeItemBase(UObject* Outer, const FItemData* ItemData, const int32 Quantity);
	
	/*ItemManager() = default;
	~ItemManager() = default;
	
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;*/
	
};
