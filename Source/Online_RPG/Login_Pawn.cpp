// Fill out your copyright notice in the Description page of Project Settings.


#include "Login_Pawn.h"

#include "Network_Manager_R.h"
#include "Sound_Manager_R.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALogin_Pawn::ALogin_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALogin_Pawn::BeginPlay()
{
	Super::BeginPlay();

	
	
	
}

// Called every frame
void ALogin_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ALogin_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

