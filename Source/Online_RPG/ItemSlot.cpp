// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"

#include "InventoryComponent.h"
#include "ItemBase.h"
#include "ItemC.h"
#include "ItemDragDrop.h"
#include "ItemDragVisual.h"
#include "ItemTooltip.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!ItemTooltipClass) return;
	
	UItemTooltip* Tooltip = CreateWidget<UItemTooltip>(this,ItemTooltipClass);
	Tooltip->SetItemSlot(this);
	SetToolTip(Tooltip);
}

void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!Item) return;
	
	PlayerInventory = PlayerCharacter->GetInventory();

	SlotImage->SetBrushFromTexture(Item->BaseItemAssetData.Icon);

	if (Item->BaseItemNumericData.bIsStackable)
	{
		SlotQuantity->SetText(FText::AsNumber(Item->BaseItemQuantity));
	}
	else
	{
		SlotQuantity->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FReply UItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
//	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(),EKeys::LeftMouseButton);
	}

	return Reply.Unhandled();
}

FReply UItemSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
//	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		Item->Use(PlayerCharacter);
		return Reply.Handled();
	}

	// submenu on right click will happen here
	return Reply.Unhandled();
}

/*void UItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}*/

void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	const TObjectPtr<UItemDragVisual> DragVisual = CreateWidget<UItemDragVisual>(this,ItemDragVisualClass);
	DragVisual->DragBorderImage->SetBrushFromTexture(Item->BaseItemAssetData.Icon);

	Item->BaseItemNumericData.bIsStackable?
		DragVisual->DragBorderQuantity->SetText(FText::AsNumber(Item->BaseItemQuantity)) :
		DragVisual->DragBorderQuantity->SetVisibility(ESlateVisibility::Collapsed);

	UItemDragDrop* DragDrop = NewObject<UItemDragDrop>();
	DragDrop->SourceItem = Item;
	DragDrop->SourceInventory = Item->OwningInventory;

	DragDrop->DefaultDragVisual = DragVisual;
	DragDrop->Pivot = EDragPivot::BottomCenter;

	OutOperation = DragDrop;
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
