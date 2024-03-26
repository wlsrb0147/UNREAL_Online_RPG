// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "InventoryComponent.h"
#include "PlayerCharacter.h"

UItemBase::UItemBase(): BaseItemQuantity(0), BaseItemType(), BaseItemAssetData(), OwningInventory(nullptr)
{
}

UItemBase* UItemBase::CreateCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->BaseItemID = this->BaseItemID;
	ItemCopy->BaseItemType = this->BaseItemType;
	ItemCopy->BaseItemQuantity = this->BaseItemQuantity;
	ItemCopy->BaseItemTextData = this->BaseItemTextData;
	ItemCopy->BaseItemNumericData = this->BaseItemNumericData;
	ItemCopy->BaseItemStatistics = this->BaseItemStatistics;
	ItemCopy->BaseItemAssetData = this->BaseItemAssetData;
	ItemCopy->bIsCopy = true;
	
	return ItemCopy;
}

// 아이템 수량이 변경됐을 때 사용함
void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity == BaseItemQuantity) return;

	if(!OwningInventory)
	{
		//UE_LOG(LogTemp,Warning,TEXT("실행"))
		BaseItemQuantity = NewQuantity;
	}
	else
	{
		BaseItemQuantity = FMath::Clamp(NewQuantity,0,
		BaseItemNumericData.bIsStackable? BaseItemNumericData.MaxStackSize:1);
	}
	
	
	if (BaseItemQuantity <= 0)
	{
		if (OwningInventory)
		{
			OwningInventory->RemoveItemFromList(this);
		}
		else
		{
			BaseItemQuantity = 1;
		}
	}
	
}

void UItemBase::Use(APlayerCharacter* UsingCharacter)
{
	if (this->BaseItemType == EItemType::Consume)
	{
		UsingCharacter->SetCurrentHealth(UsingCharacter->GetCurrentHealth()+this->BaseItemStatistics.HealAmount); 
		OwningInventory->RemoveAmountOfItem(this,1);
	}
	else
	{
		//UE_LOG(LogTemp,Display,TEXT("Use 실행"))
	}
}
