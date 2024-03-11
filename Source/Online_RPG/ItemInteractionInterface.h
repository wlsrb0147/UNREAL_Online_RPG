// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInteractionInterface.generated.h"

class AItemC;

UENUM()
enum class EInteractionType:uint8
{
	PickUp UMETA(DisplayName = "PickUp"),
	NPC UMETA(DisplayName = "NPC"),
	Button UMETA(DisplayName = "Button")
};

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData(): InteractionType(EInteractionType::PickUp),
	Name(FText::GetEmpty()),
	Action(FText::GetEmpty()),
	Quantity(0),
	InteractionDuration(0.0f)
	{
		
	}

	UPROPERTY(EditInstanceOnly, Category ="ItemData")
	EInteractionType InteractionType;

	UPROPERTY(EditInstanceOnly, Category ="ItemData")
	FText Name;

	UPROPERTY(EditInstanceOnly, Category ="ItemData")
	FText Action;

	UPROPERTY(EditInstanceOnly, Category ="ItemData")
	int32 Quantity;

	UPROPERTY(EditInstanceOnly, Category ="ItemData")
	float InteractionDuration;
	
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInteractionInterface : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class ONLINE_RPG_API IItemInteractionInterface
{
	GENERATED_BODY()


public:
	FInteractionData InteractionData;

	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void Interact(AItemC* PlayerCharacter);
	virtual void EndInteract();


};
