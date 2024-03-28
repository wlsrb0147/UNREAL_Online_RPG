// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PotionSpawner.generated.h"

class APickUpItem;

UCLASS()
class ONLINE_RPG_API APotionSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APotionSpawner();

	UPROPERTY(EditAnywhere)
	TSubclassOf<APickUpItem> Pickup_Class;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

};
