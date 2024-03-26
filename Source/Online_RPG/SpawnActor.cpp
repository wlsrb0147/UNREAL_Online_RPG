// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActor.h"

#include "AItemManager.h"
#include "Network_Manager_R.h"
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
	InstanceInteractionData.Name = FText::FromString(TEXT("거렁뱅이"));
	InstanceInteractionData.Quantity = 1;
	InstanceInteractionData.InteractionType = EInteractionType::NPC;

	InteractionData = InstanceInteractionData;
}
