// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UItemBase : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Category = "Item Data")
	int32 BaseItemQuantity;
	
	UPROPERTY(EditAnywhere,Category = "Item Data")
	FName BaseItemID;

	UPROPERTY(EditAnywhere,Category = "Item Data")
	EItemType BaseItemType;
	
	UPROPERTY(EditAnywhere,Category = "Item Data")
	FItemStatistics BaseItemStatistics;

	UPROPERTY(EditAnywhere,Category = "Item Data")
	FItemTextData BaseItemTextData;
	
	UPROPERTY(EditAnywhere,Category = "Item Data")
	FItemNumericData BaseItemNumericData;
	
	UPROPERTY(EditAnywhere,Category = "Item Data")
	FItemAssetData BaseItemAssetData;

protected:

	bool operator==(const FName& OtherID) const
	{
		return this->BaseItemID == OtherID;
	}
};
