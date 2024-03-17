// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDragVisual.generated.h"

class UTextBlock;
class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UItemDragVisual : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UBorder* DragBorder;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* DragBorderImage;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* DragBorderQuantity;
	
};
