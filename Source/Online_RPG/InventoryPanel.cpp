// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPanel.h"
#include "ItemSlot.h"
#include "InventoryComponent.h"
#include "ItemC.h"
#include "Components/WrapBox.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	PlayerCharacter = Cast<AItemC>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;
	
	Inventory = PlayerCharacter->GetInventory();
	if (!Inventory) return;
	
	Inventory->OnInventoryUpdated.AddUObject(this,&UInventoryPanel::RefreshInventory);
	WriteInfoText();
}

void UInventoryPanel::RefreshInventory()
{
	if (!Inventory || !InventorySlotClass) return;

	InventoryContents->ClearChildren();
	
	for (UItemBase* const& InventoryItem : Inventory->GetInventory())
	{
		UItemSlot* ItemSlot = CreateWidget<UItemSlot>(this,InventorySlotClass);
		ItemSlot->SetItem(InventoryItem);
		ItemSlot->SetPlayer(PlayerCharacter);
		InventoryContents->AddChildToWrapBox(ItemSlot);

		
	}

	WriteInfoText();
}

void UInventoryPanel::WriteInfoText() const
{
	// 돈 갱신
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
