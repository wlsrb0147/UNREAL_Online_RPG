// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryHUD.h"

#include "InventoryMainMenu.h"
#include "ItemInteractWidget.h"

void AInventoryHUD::OpenInventoryWidget()
{
	bIsInventoryOpen = !bIsInventoryOpen;
	InventoryMainMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void AInventoryHUD::CloseInventoryWidget()
{
	bIsInventoryOpen = !bIsInventoryOpen;
	InventoryMainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
}


void AInventoryHUD::ToggleInventoryWidget()
{
	if (bIsInventoryOpen)
	{
		CloseInventoryWidget();
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		OpenInventoryWidget();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void AInventoryHUD::OpenInteractionWidget() const
{
	if (!ItemInteractionWidget) return;
	ItemInteractionWidget->SetVisibility(ESlateVisibility::Visible);
}

void AInventoryHUD::CloseInteractionWidget() const
{
	if (!ItemInteractionWidget) return;
	ItemInteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AInventoryHUD::UpdateInteractionWidget(const FInteractionData* InteractionData) const
{
	if (!ItemInteractionWidget) return;
	
	if (ItemInteractionWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		ItemInteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
	ItemInteractionWidget->UpdateWidget(InteractionData);
	
}

void AInventoryHUD::BeginPlay()
{
	Super::BeginPlay();

	
	if (ItemInteractionWidgetClass)
	{
		ItemInteractionWidget = CreateWidget<UItemInteractWidget>(GetWorld(),ItemInteractionWidgetClass);
		ItemInteractionWidget->AddToViewport(-1);
		ItemInteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InventoryMainMenuClass)
	{
		InventoryMainMenuWidget = CreateWidget<UInventoryMainMenu>(GetWorld(),InventoryMainMenuClass);
		InventoryMainMenuWidget->AddToViewport(5);
		InventoryMainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		InventoryPanel = InventoryMainMenuWidget->GetInventoryPanel();
	}

}
