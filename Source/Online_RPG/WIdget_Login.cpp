// Fill out your copyright notice in the Description page of Project Settings.


#include "WIdget_Login.h"

#include "Network_Manager_R.h"

void UWIdget_Login::JoinClick()
{
	UE_LOG(LogTemp, Log, TEXT("Join CLick!!!!"));

	
}

void UWIdget_Login::LoginClick(FString id, FString pw)
{
	UE_LOG(LogTemp, Log, TEXT("Login CLick!!!! %s %s"), *id, *pw);
	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());

	Instance->SelectUser(id,pw);
	
}

void UWIdget_Login::ResistClick(FString id, FString pw, FString nickname)
{
	UE_LOG(LogTemp, Log, TEXT("Resist CLick!!!! %s %s %s"), *id, *pw, *nickname);
	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());

	Instance->ResistUser(id,pw, nickname);
}

// void UWIdget_Login::LoginClick()
// {
// 	UE_LOG(LogTemp, Log, TEXT("Login CLick!!!!"));
// 	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());
// 	
// 	
// }
