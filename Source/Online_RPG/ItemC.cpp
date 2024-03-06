// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemC.h"

// Sets default values
AItemC::AItemC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AItemC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AItemC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

