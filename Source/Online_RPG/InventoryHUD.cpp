// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryHUD.h"

#include "InventoryMainMenu.h"
#include "ItemInteractWidget.h"

void AInventoryHUD::OpenInventoryWidget()
{
	InventoryMainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	GetOwningPlayerController()->SetShowMouseCursor(true);
	bIsInventoryOpen = !bIsInventoryOpen;
}

void AInventoryHUD::CloseInventoryWidget()
{
	InventoryMainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayerController()->SetShowMouseCursor(false);
	bIsInventoryOpen = !bIsInventoryOpen;
}


void AInventoryHUD::ToggleInventoryWidget()
{
	if (!bIsInventoryOpen)
	{
		OpenInventoryWidget();
	}
	else
	{
		CloseInventoryWidget();
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
	}

}
