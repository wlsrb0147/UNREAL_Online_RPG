// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInteractWidget.h"

#include "ItemInteractionInterface.h"
#include "Components/TextBlock.h"

void UItemInteractWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//InteractionProgressBar->PercentDelegate.BindUFunction(this,"")
}

void UItemInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemBut->SetText(FText::FromString(TEXT("버튼을")));

}

void UItemInteractWidget::UpdateWidget(const FInteractionData* InteractionData) const
{
	switch (InteractionData->InteractionType) {
	case EInteractionType::PickUp:
		ItemX->SetText(FText::FromString("x"));
		ItemNum->SetText(FText::AsNumber(InteractionData->Quantity));
		ItemInteractionMethod->SetText(FText::FromString(TEXT("획득")));

		ItemX->SetVisibility(ESlateVisibility::Visible);
		ItemNum->SetVisibility(ESlateVisibility::Visible);

		break;

	case EInteractionType::NPC:
		ItemX->SetVisibility(ESlateVisibility::Collapsed);
		ItemNum->SetVisibility(ESlateVisibility::Collapsed);
		ItemInteractionMethod->SetText(FText::FromString(TEXT("대화하기")));

		break;

	case EInteractionType::Button:
		ItemX->SetText(FText::FromString(TEXT("버튼")));
		ItemNum->SetVisibility(ESlateVisibility::Collapsed);
		ItemInteractionMethod->SetText(FText::FromString(TEXT("작동")));

		ItemX->SetVisibility(ESlateVisibility::Visible);

		break;
	default:;
		//////UE_LOG(LogTemp,Error,TEXT("default"));
	}
	ItemName->SetText(InteractionData->Name);
	ItemButton->SetText(FText::FromString("F"));
}


