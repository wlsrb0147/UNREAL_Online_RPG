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
	// 인벤토리에서의 수량
	UPROPERTY(EditAnywhere,Category = "Item Data")
	int32 BaseItemQuantity;
	
	UPROPERTY(VisibleAnywhere,Category = "Item Data")
	FName BaseItemID;

	UPROPERTY(VisibleAnywhere,Category = "Item Data")
	EItemType BaseItemType;
	
	UPROPERTY(VisibleAnywhere,Category = "Item Data")
	FItemStatistics BaseItemStatistics;

	UPROPERTY(VisibleAnywhere,Category = "Item Data")
	FItemTextData BaseItemTextData;
	
	UPROPERTY(VisibleAnywhere,Category = "Item Data")
	FItemNumericData BaseItemNumericData;
	
	UPROPERTY(VisibleAnywhere,Category = "Item Data")
	FItemAssetData BaseItemAssetData;

protected:

	bool operator==(const FName& OtherID) const
	{
		return this->BaseItemID == OtherID;
	}
};
