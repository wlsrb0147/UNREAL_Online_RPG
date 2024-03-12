// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InventoryHUD.generated.h"

class UInventoryMainMenu;
struct FInteractionData;
class UItemInteractWidget;
/**
 * 
 */
UCLASS()
class ONLINE_RPG_API AInventoryHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,Category = "Widgets")
	TSubclassOf<UItemInteractWidget> ItemInteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category = "Widgets")
	TSubclassOf<UInventoryMainMenu> InventoryMainMenuClass;
	
	UPROPERTY()
	UItemInteractWidget* ItemInteractionWidget;

	UPROPERTY()
	UInventoryMainMenu* InventoryMainMenuWidget;
	
	bool bIsInventoryOpen = false;

	void ToggleInventoryWidget();
	void OpenInventoryWidget();
	void CloseInventoryWidget();
	
	void OpenInteractionWidget() const;
	void CloseInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractionData* InteractionData) const;

protected:
	virtual void BeginPlay() override;
};
