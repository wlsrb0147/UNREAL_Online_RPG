// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "SpawnActor.generated.h"

class AItemManager;

UCLASS()
class ONLINE_RPG_API ASpawnActor : public AActor, public IItemInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnActor();
	FInteractionData InstanceInteractionData;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void BeginInteract() override;
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	FBum GetFBum() const {return ConversationData;}

	UPROPERTY(EditAnywhere, Category = "Initial Value")
	FBum InitialBum;
};
