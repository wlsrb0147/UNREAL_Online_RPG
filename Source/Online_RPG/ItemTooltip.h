// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTooltip.generated.h"

class UItemSlot;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UItemTooltip : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetItemSlot(UItemSlot* CurrentSlot) {HoveredInventorySlot = CurrentSlot;}
	
protected:
	UPROPERTY(VisibleAnywhere)
	UItemSlot* HoveredInventorySlot;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* TooltipName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* TooltipUsage;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* TooltipAtk;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* TooltipDef;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* TooltipExplain;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* TooltipStack;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* TooltipSell;
};
