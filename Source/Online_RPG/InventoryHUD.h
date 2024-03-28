// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InventoryHUD.generated.h"

class UNetwork_Manager_R;
class APlayerCharacter;
struct FBum;
class UNPCConversation;
class UInventoryPanel;
class UInventoryMainMenu;
struct FInteractionData;
class UItemInteractWidget;
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FInitializeWidget);

UCLASS()
class ONLINE_RPG_API AInventoryHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	FInitializeWidget InitializeWidget;
	
	UPROPERTY(EditDefaultsOnly,Category = "Widgets")
	TSubclassOf<UItemInteractWidget> ItemInteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category = "Widgets")
	TSubclassOf<UInventoryMainMenu> InventoryMainMenuClass;

	UPROPERTY(EditDefaultsOnly,Category = "widgets")
	TSubclassOf<UNPCConversation> NPCConversationClass;
	
	UPROPERTY()
	UItemInteractWidget* ItemInteractionWidget;

	UPROPERTY()
	UInventoryMainMenu* InventoryMainMenuWidget;

	UPROPERTY()
	UInventoryPanel* InventoryPanel;

	UPROPERTY()
	UNPCConversation* NPCConversation;

	UPROPERTY()
	UUserWidget* SkillWidget;

	void SetSkillWidget(UUserWidget* GetWidget){SkillWidget = GetWidget;}
	
	bool bIsConversationOpen = false;
	bool bIsInventoryOpen = false;

	UPROPERTY()
	UNetwork_Manager_R* NetworkManager;

	void ToggleInventoryWidget();
	void OpenInventoryWidget();
	void CloseInventoryWidget();
	
	void OpenInteractionWidget() const;
	void CloseInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractionData* InteractionData) const;

	void OpenConversationWidget(const FBum& Initial);
	void CloseConversationWidget();
	
protected:
	virtual void BeginPlay() override;
};
