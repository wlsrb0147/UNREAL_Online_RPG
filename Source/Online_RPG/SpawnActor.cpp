// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActor.h"
#include "PickUpItem.h"

class UItemBase;
// Sets default values
ASpawnActor::ASpawnActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASpawnActor::BeginPlay()
{
	Super::BeginPlay();

	InitialBum.ConversationText.Add(FText::FromString(TEXT("배고파")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("밥줘")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("심심해")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("뭐하지")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("되냐?")));

	ConversationData = InitialBum;
	
	InstanceInteractionData.Quantity = 1;
	InstanceInteractionData.InteractionType = EInteractionType::NPC;
	InstanceInteractionData.Name = FText::FromString(TEXT("거렁뱅이"));

	InteractionData = InstanceInteractionData;
}

void ASpawnActor::BeginInteract()
{
	IItemInteractionInterface::BeginInteract();
}

void ASpawnActor::Interact(APlayerCharacter* PlayerCharacter)
{
	IItemInteractionInterface::Interact(PlayerCharacter);
}
