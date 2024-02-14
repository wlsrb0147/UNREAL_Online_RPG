// Fill out your copyright notice in the Description page of Project Settings.


#include "WIdget_Login.h"

#include "Network_Manager_R.h"

void UWIdget_Login::JoinClick()
{
	UE_LOG(LogTemp, Log, TEXT("Join CLick!!!!"));

	
}

void UWIdget_Login::LoginClick(FString _id, FString _pw)
{
	UE_LOG(LogTemp, Log, TEXT("Login CLick!!!! %s %s"), *_id, *_pw);
	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());

	Instance->SelectUser(_id,_pw);
	
}

void UWIdget_Login::ResistClick(FString _id, FString _pw, FString nickname)
{
	UE_LOG(LogTemp, Log, TEXT("Resist CLick!!!! %s %s %s"), *_id, *_pw, *nickname);
	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());

	Instance->ResistUser(_id,_pw, nickname);
}

// void UWIdget_Login::LoginClick()
// {
// 	UE_LOG(LogTemp, Log, TEXT("Login CLick!!!!"));
// 	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());
// 	
// 	
// }
