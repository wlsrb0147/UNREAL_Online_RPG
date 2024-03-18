// Fill out your copyright notice in the Description page of Project Settings.


#include "GreekIsland_GameMode.h"

#include "LoginController.h"
#include "Network_Manager_R.h"

void AGreekIsland_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALoginController* NewController = Cast<ALoginController>(NewPlayer);

	NewController->INDEX_OF_PLAYER_CONTROLLER = ++Unique_Player_Idx;
	UE_LOG(LogTemp, Warning, TEXT("Post Login complete .... INDEX : %d, NewPlayer : %s"), NewController->INDEX_OF_PLAYER_CONTROLLER, *NewPlayer->GetName());
}

void AGreekIsland_GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	UE_LOG(LogTemp, Warning, TEXT("Someone Log out "));


	// ALoginController* Controller =  Cast<ALoginController>(Exiting);
	// if(Controller)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("ID %s 가 로그아웃"), *Controller->Login_ID);
	// 	UE_LOG(LogTemp, Log, TEXT("Player %s is disconnecting from location: %s"), *GetName(), *Controller->Current_Location.ToString());
	// 	UE_LOG(LogTemp, Log, TEXT("Player %s is disconnecting from rotation: %s"), *GetName(), *Controller->Current_Rotatation.ToString());
	//
	//
	// 	if(Controller->Current_Location.IsNearlyZero()) return;
	// 	UNetwork_Manager_R* Network_Manager_R = Cast<UNetwork_Manager_R>(GetGameInstance());
	// 	Network_Manager_R->SetSpawnData(Controller->Current_Location, Controller->Current_Rotatation, Controller->Login_ID);
	// }
	// // 플레이어 캐릭터(폰)의 참조를 얻습니다.
	// APawn* PlayerPawn = Exiting->GetPawn();
	// if (PlayerPawn)
	// {
	// 	// 플레이어 캐릭터의 위치를 얻습니다.
	// 	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	// 	UE_LOG(LogTemp, Log, TEXT("Player %s is disconnecting from location: %s"), *GetName(), *PlayerLocation.ToString());
	// }else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Pawn null "));
	// 	
	// }
	
}
