// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageAble.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONLINE_RPG_API UDamageAble : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageAble();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetHealth();

	bool GetIsDead();

	void TakeDamage(float Damage);

private:
	UPROPERTY()
	float Health;

	UPROPERTY()
	float MaxHealth = 100;

	UPROPERTY()
	bool IsDead = false;

	
};
