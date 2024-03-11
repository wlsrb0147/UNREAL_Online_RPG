// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Itemstruct.generated.h"


UENUM(BlueprintType)
enum class EItemType:uint8
{
	LeftHand UMETA(DisplayName = "LeftHand" ),
	RightHand UMETA(DisplayName = "RightHand" ),
	Clothes UMETA(DisplayName = "RightHand" ),
	Consume UMETA(DisplayName = "Consume" ),
	Quest UMETA(DisplayName = "Quest" ),
	Event UMETA(DisplayName = "Event" ),
	Trash UMETA(DisplayName = "Trash" ),
	Money UMETA(DisplayName = "Money" )
};

USTRUCT()
struct FItemStatistics
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	float ArmorPoint = 0;
	
	UPROPERTY(EditDefaultsOnly)
	float AttackPoint = 0;
	
	UPROPERTY(EditDefaultsOnly)
	float HealAmount = 0;

	UPROPERTY(EditDefaultsOnly)
	float BuyValue = 0;
	
	UPROPERTY(EditDefaultsOnly)
	float SellValue = 0;
};

USTRUCT()
struct FItemTextData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FText NameText;
	UPROPERTY(EditAnywhere)
	FText ItemTypeText;
	UPROPERTY(EditAnywhere)
	FText DescriptionText;
	UPROPERTY(EditAnywhere)
	FText InteractionText;
	
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	int32 MaxStackSize = 0;
	
	UPROPERTY(EditAnywhere)
	bool bIsStackable = false;
	
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
	
};

USTRUCT()
struct FItemData:public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,Category = "Item Data")
	FName ItemID;

	UPROPERTY(EditAnywhere,Category = "Item Data")
	EItemType ItemType = EItemType::Trash;
	
	UPROPERTY(EditAnywhere,Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere,Category = "Item Data")
	FItemTextData ItemTextData;
	
	UPROPERTY(EditAnywhere,Category = "Item Data")
	FItemNumericData ItemNumericData;
	
	UPROPERTY(EditAnywhere,Category = "Item Data")
	FItemAssetData ItemAssetData;
};

class ONLINE_RPG_API ItemStruct
{
public:
	ItemStruct();
	~ItemStruct();
};
