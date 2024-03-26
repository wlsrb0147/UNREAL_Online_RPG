// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCConversation.h"

#include "AItemManager.h"
#include "InventoryComponent.h"
#include "InventoryHUD.h"
#include "ItemInteractionInterface.h"
#include "Network_Manager_R.h"
#include "PlayerCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UNPCConversation::NativeConstruct()
{
	Super::NativeConstruct();

	UNetwork_Manager_R* Network_Manager =  Cast<UNetwork_Manager_R>(GetGameInstance());
	//const ItemManager& ItemManagerInstance = ItemManager::Get();

	ItemManagerInstance = Network_Manager->GetItemManager();
	QuestItem = ItemManagerInstance->MakeItemBaseByKey(this,7,1);
}

FReply UNPCConversation::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UpdateWidget();
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UNPCConversation::InitializeWidget(const FBum& InitialBum)
{
	FaceImg->SetBrushFromTexture(InitialBum.Face);
	NameText->SetText(InitialBum.Name);
	CurrentTalking.Face = InitialBum.Face;
	CurrentTalking.Name = InitialBum.Name;
	CurrentTalking.ConversationText = InitialBum.ConversationText;
	
	if (CurrentCharacter->IsQuestAccept == 0)
	{
		ContextNum = 0;
	}
	else if (CurrentCharacter->IsQuestAccept == 1)
	{
		ContextNum = 4;
	}
	else if (CurrentCharacter->IsQuestAccept == 2)
	{
		ContextNum = 8;
	}
	UpdateWidget();
}

void UNPCConversation::UpdateWidget()
{
	if (CurrentCharacter->IsQuestAccept == 0)
	{
		if (ContextNum == 4)
		{
			CurrentCharacter->IsQuestAccept = 1;
			CurrentCharacter->FoundNoInteract();
			return;
		}
		ConText->SetText(CurrentTalking.ConversationText[ContextNum]);
		++ContextNum;
		return;
	}

	if (CurrentCharacter->IsQuestAccept == 1)
	{
		if (ContextNum >= 5 && !InventoryItem)
		{
			ConText->SetText(CurrentTalking.ConversationText[ContextNum]);
			++ContextNum;
			if (ContextNum == 7)
			{
				ContextNum = 4;
				CurrentCharacter->FoundNoInteract();
			}
			return;
		}
		
		if (ContextNum >= 5 && InventoryItem)
		{
			++ContextNum;
			if (ContextNum == 8)
			{
				CurrentCharacter->IsQuestAccept = 2;
				CurrentCharacter->FoundNoInteract();
				return;
			}
			ConText->SetText(CurrentTalking.ConversationText[ContextNum]);
			return;
		}
		
		ConText->SetText(CurrentTalking.ConversationText[ContextNum]);
		InventoryItem = CurrentCharacter->GetInventory()->FindNextPartial(QuestItem);
		++ContextNum;
	}

	if (CurrentCharacter->IsQuestAccept == 2)
	{
		if (ContextNum == 9)
		{
			CurrentCharacter->FoundNoInteract();
			return;
		}
		ConText->SetText(CurrentTalking.ConversationText[ContextNum]);
		++ContextNum;
	}
}

