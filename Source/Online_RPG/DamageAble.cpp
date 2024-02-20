// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageAble.h"

// Sets default values for this component's properties
UDamageAble::UDamageAble()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDamageAble::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	IsDead = false;
}
float UDamageAble::GetHealth()
{
	return Health;
}
bool UDamageAble::GetIsDead()
{
	return IsDead;
}

void UDamageAble::TakeDamage(float Damage)
{
	Health -= Damage;
	if (Health < 0) Health = 0;
}

// Called every frame
void UDamageAble::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

