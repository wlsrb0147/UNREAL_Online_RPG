// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPanel.h"
#include "ItemSlot.h"
#include "InventoryComponent.h"
#include "ItemC.h"
#include "ItemDragDrop.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

void UInventoryPanel::InitializePanel(APlayerCharacter* Player)
{
	
	PlayerCharacter = Player;
	if (!PlayerCharacter) return;

	Inventory = PlayerCharacter->GetInventory();
	if (!Inventory) return;
	
	
	Inventory->OnInventoryUpdated.AddUObject(this,&UInventoryPanel::RefreshInventory);
	WriteInfoText();
	UE_LOG(LogTemp,Warning,TEXT("패널 이니셜라이즈 성공"))
	ActiveInitialize = true;
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
	MoneyText->SetText(FText::AsNumber(Inventory->GetMoney()));
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
//	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	const UItemDragDrop* ItemDragDrop = Cast<UItemDragDrop>(InOperation);

	if (ItemDragDrop->SourceItem && Inventory)
	{
		return  true;
	}

	return false;
	
}

