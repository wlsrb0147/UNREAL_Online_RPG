// Fill out your copyright notice in the Description page of Project Settings.


#include "GreekIsland_GameMode.h"

#include "LoginController.h"

void AGreekIsland_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALoginController* NewController = Cast<ALoginController>(NewPlayer);

	NewController->INDEX_OF_PLAYER_CONTROLLER = ++Unique_Player_Idx;
	UE_LOG(LogTemp, Log, TEXT("Post Login complete .... %d"), NewController->INDEX_OF_PLAYER_CONTROLLER);
}
