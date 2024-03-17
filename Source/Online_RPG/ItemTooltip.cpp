// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemTooltip.h"

#include "ItemBase.h"
#include "ItemSlot.h"
#include "Components/TextBlock.h"

void UItemTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* HoveredItem = HoveredInventorySlot->GetItem();

	switch (HoveredItem->BaseItemType) {
	case EItemType::LeftHand:
		TooltipUsage->SetText(FText::FromString(TEXT("왼손 장비")));
		TooltipAtk->SetText( FText::AsNumber(HoveredItem->BaseItemStatistics.AttackPoint));
		TooltipDef->SetText( FText::AsNumber(HoveredItem->BaseItemStatistics.ArmorPoint));
		break;
	case EItemType::RightHand:
		TooltipUsage->SetText(FText::FromString(TEXT("오른손 장비")));
		TooltipAtk->SetText( FText::AsNumber(HoveredItem->BaseItemStatistics.AttackPoint));
		TooltipDef->SetText( FText::AsNumber(HoveredItem->BaseItemStatistics.ArmorPoint));
		break;
	case EItemType::Clothes:
		TooltipUsage->SetText(FText::FromString(TEXT("옷")));
		TooltipAtk->SetText( FText::AsNumber(HoveredItem->BaseItemStatistics.AttackPoint));
		TooltipDef->SetText( FText::AsNumber(HoveredItem->BaseItemStatistics.ArmorPoint));
		break;
	case EItemType::Consume:
		TooltipUsage->SetText(FText::FromString(TEXT("소비")));
		break;
	case EItemType::Quest:
		TooltipUsage->SetText(FText::FromString(TEXT("퀘스트")));
		break;
	case EItemType::Event:
		TooltipUsage->SetText(FText::FromString(TEXT("이벤트")));
		break;
	case EItemType::Trash:
		TooltipUsage->SetText(FText::FromString(TEXT("기타")));
		break;
	case EItemType::Money:
		TooltipUsage->SetText(FText::FromString(TEXT("돈")));
		break;
	default:
		TooltipUsage->SetText(FText::FromString(TEXT("미할당")));;
	}

	TooltipName->SetText(HoveredItem->BaseItemTextData.NameText);
	TooltipExplain->SetText(HoveredItem->BaseItemTextData.DescriptionText);

	const FText MaxStackText = FText::Format(NSLOCTEXT("ItemTooltip","MaxStackSizeText","최대 소지량 : {0}"),
	                                         FText::AsNumber(HoveredItem->BaseItemNumericData.MaxStackSize));
	TooltipStack->SetText(MaxStackText);

	const FText SellText = FText::Format(NSLOCTEXT("ItemTooltip","SellPrice","판매 가격 : {0}원"),
											 FText::AsNumber(HoveredItem->BaseItemStatistics.SellValue));
	TooltipSell->SetText(SellText);
	
	if (HoveredItem->BaseItemStatistics.AttackPoint == 0)
	{
		TooltipAtk->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (HoveredItem->BaseItemStatistics.ArmorPoint == 0)
	{
		TooltipDef->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (HoveredItem->BaseItemNumericData.MaxStackSize == 0)
	{
		TooltipStack->SetVisibility(ESlateVisibility::Collapsed);
	}
}
