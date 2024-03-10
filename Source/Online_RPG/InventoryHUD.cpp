// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryHUD.h"

#include "ItemInteractWidget.h"

void AInventoryHUD::OpenInventoryWidget()
{
}

void AInventoryHUD::CloseInventoryWidget()
{
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
	UE_LOG(LogTemp,Error,TEXT("못찾음"))
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

}
