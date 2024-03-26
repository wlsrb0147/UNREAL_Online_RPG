// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCConversation.h"

#include "InventoryHUD.h"
#include "ItemInteractionInterface.h"
#include "PlayerCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UNPCConversation::NativeConstruct()
{
	Super::NativeConstruct();
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
	ContextNum = 0;
	FaceImg->SetBrushFromTexture(InitialBum.Face);
	NameText->SetText(InitialBum.Name);
	CurrentTalking.Face = InitialBum.Face;
	CurrentTalking.Name = InitialBum.Name;
	CurrentTalking.ConversationText = InitialBum.ConversationText;
	NextTalk = true;
	UpdateWidget();
}

void UNPCConversation::UpdateWidget()
{
	if (ContextNum < CurrentTalking.ConversationText.Num())
	{
		ConText->SetText(CurrentTalking.ConversationText[ContextNum]);
		++ContextNum;
		return;
	}

	CurrentCharacter->FoundNoInteract();
}

