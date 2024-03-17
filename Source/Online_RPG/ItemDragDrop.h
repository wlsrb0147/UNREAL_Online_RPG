// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDrop.generated.h"

class UInventoryComponent;
class UItemBase;
/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UItemDragDrop : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UItemBase* SourceItem;
	UPROPERTY()
	UInventoryComponent* SourceInventory;
	
};
