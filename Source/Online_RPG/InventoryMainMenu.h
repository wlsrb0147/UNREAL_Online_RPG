// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainMenu.generated.h"

class UInventoryPanel;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UInventoryMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

public:
	UInventoryPanel* GetInventoryPanel() const {return InventoryPanel;}
	
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UInventoryPanel* InventoryPanel;

protected:
	
};
