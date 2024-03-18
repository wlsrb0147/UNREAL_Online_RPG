// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

class UInventoryComponent;
class APlayerCharacter;
class UItemTooltip;
class UItemDragVisual;
class UItemBase;
class UTextBlock;
class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemBase* GetItem() const{ return Item; }
	void SetItem(UItemBase* ItemRef){Item = ItemRef;}
	void SetPlayer(APlayerCharacter* CharacterRef){PlayerCharacter = CharacterRef;}

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
//	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

	UPROPERTY(VisibleAnywhere)
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemDragVisual> ItemDragVisualClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemTooltip> ItemTooltipClass;
	
	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* PlayerInventory;
	
	UPROPERTY(VisibleAnywhere)
	UItemBase* Item;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UBorder* SlotBorder;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* SlotImage;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* SlotQuantity;
};
