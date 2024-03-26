// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInteractionInterface.h"
#include "Blueprint/UserWidget.h"
#include "NPCConversation.generated.h"

class UItemBase;
class AItemManager;
class AInventoryHUD;
struct FBum;
class UTextBlock;
class UImage;
/**
 * 
 */

UCLASS()
class ONLINE_RPG_API UNPCConversation : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	void InitializeWidget(const FBum& InitialBum);
	void UpdateWidget();
	void SetCharacter(APlayerCharacter* Chara){CurrentCharacter = Chara;}
	
	int8 ContextNum;
	
	UPROPERTY(meta = (BindWidget))
	UImage* FaceImg;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConText;

	UPROPERTY()
	FBum CurrentTalking;

	bool NextTalk = true;

	UPROPERTY()
	APlayerCharacter* CurrentCharacter;

	UPROPERTY()
	AItemManager* ItemManagerInstance;

	UPROPERTY()
	UItemBase* QuestItem;

	UPROPERTY()
	UItemBase* InventoryItem = nullptr;
	
};
