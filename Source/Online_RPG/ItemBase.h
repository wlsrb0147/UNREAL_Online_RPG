// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.h"
#include "ItemBase.generated.h"

class UInventoryComponent;
class AItemC;
/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UItemBase : public UObject
{
	GENERATED_BODY()
public:
	UItemBase();
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

	bool bIsCopy = false;
	bool bIsPickUp = false;
	void ResetItemFlags(){bIsCopy = false, bIsPickUp =false;}
	UItemBase* CreateCopy() const;

	UPROPERTY()
	UInventoryComponent* OwningInventory;
	
	void SetQuantity(const int32 NewQuantity);
	bool IsFullItemStack() const {return BaseItemQuantity == BaseItemNumericData.MaxStackSize;}
	void Use(AItemC* UsingCharacter);
	
	
protected:

	bool operator==(const FName& OtherID) const
	{
		return this->BaseItemID == OtherID;
	}
};
