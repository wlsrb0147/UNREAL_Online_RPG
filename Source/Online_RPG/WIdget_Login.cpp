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

	Instance->SelectUser(_id,_pw, this);
	
}

void UWIdget_Login::ResistClick(FString _id, FString _pw, FString nickname)
{
	UE_LOG(LogTemp, Log, TEXT("Resist CLick!!!! %s %s %s"), *_id, *_pw, *nickname);
	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());

	Instance->ResistUser(_id,_pw, nickname);
}

void UWIdget_Login::LoginSuccess()
{
	UE_LOG(LogTemp, Log, TEXT("Login Success"));
}

void UWIdget_Login::LoginFail()
{
	UE_LOG(LogTemp, Log, TEXT("Login Fail"));
}

void UWIdget_Login::SetCurrentWidget(AActor* Widget)
{
	UUserWidget* InputWidget = Cast<UUserWidget>(Widget);
	if(InputWidget) Current_Widget = InputWidget;
}

void UWIdget_Login::SetSoonDestroy()
{
	FTimerHandle FTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this,  &UWIdget_Login::SetDestroy, 2, false );
	//World->GetTimerManager().SetTimer(FiringTimer, this, &AOnline_RPGCharacter::StopFire, FireRate, false);
}

void UWIdget_Login::SetDestroy()
{
	RemoveFromParent();
}



// void UWIdget_Login::LoginClick()
// {
// 	UE_LOG(LogTemp, Log, TEXT("Login CLick!!!!"));
// 	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());
// 	
// 	
// }
