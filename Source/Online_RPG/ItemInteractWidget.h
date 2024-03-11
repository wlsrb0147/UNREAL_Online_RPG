// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "ItemInteractWidget.generated.h"

struct FInteractionData;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UItemInteractWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* ItemX;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* ItemNum;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* ItemBut;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* ItemButton;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* ItemInteractionMethod;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UProgressBar* InteractionProgressBar;

	void UpdateWidget(const FInteractionData* InteractionData) const;
};
