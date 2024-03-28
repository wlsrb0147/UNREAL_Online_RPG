// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionSpawner.h"

#include "AItemManager.h"
#include "ItemC.h"
#include "Network_Manager_R.h"

// Sets default values
APotionSpawner::APotionSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APotionSpawner::BeginPlay()
{
	Super::BeginPlay();

	UNetwork_Manager_R* NetworkManager = Cast<UNetwork_Manager_R>(GetGameInstance());
	AItemManager* ItemManager = NetworkManager->GetItemManager();

	UItemBase* ItemBase = ItemManager->MakeItemBaseByKey(this,4,10);
	ItemManager->SpawnItem(this,ItemBase,GetActorTransform(),300);
}
