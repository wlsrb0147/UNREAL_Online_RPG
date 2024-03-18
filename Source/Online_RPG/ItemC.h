// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "ItemC.generated.h"

class UItemBase;
class UInventoryComponent;
class IItemInteractionInterface;
class AInventoryHUD;

UCLASS()
class ONLINE_RPG_API AItemC : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AItemC();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
