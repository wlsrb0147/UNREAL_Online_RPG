// Fill out your copyright notice in the Description page of Project Settings.


#include "WIdget_Login.h"

#include "Network_Manager_R.h"

void UWIdget_Login::JoinClick()
{
	UE_LOG(LogTemp, Log, TEXT("Join CLick!!!!"));

	
}

void UWIdget_Login::LoginClick()
{
	UE_LOG(LogTemp, Log, TEXT("Login CLick!!!!"));
	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());
	
	
}
