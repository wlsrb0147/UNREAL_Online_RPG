// Fill out your copyright notice in the Description page of Project Settings.


#include "WIdget_Login.h"

#include "LoginController.h"
#include "Network_Manager_R.h"
#include "Kismet/KismetSystemLibrary.h"

void UWIdget_Login::JoinClick()
{
	//UE_LOG(LogTemp, Log, TEXT("Join CLick!!!!"));
}

bool UWIdget_Login::NeedSetButtonCool()
{
	if(bIsButtonCool) return false;
	bIsButtonCool = true;
	GetWorld()->GetTimerManager().SetTimer(ButtonCoolTimer, this, &UWIdget_Login::StopButtonCool, 0.3f, false );
	return true;
}

void UWIdget_Login::StopButtonCool()
{
	bIsButtonCool = false;
}

void UWIdget_Login::LoginClick(FString _id, FString _pw)
{
	if(!NeedSetButtonCool()) return;
	//UE_LOG(LogTemp, Log, TEXT("Login CLick!!!! %s %s"), *_id, *_pw);
	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());

	Instance->SelectUser(_id,_pw, this);
}

void UWIdget_Login::ResistClick(FString _id, FString _pw, FString nickname)
{
	if(!NeedSetButtonCool()) return;
	//UE_LOG(LogTemp, Log, TEXT("Resist CLick!!!! %s %s %s"), *_id, *_pw, *nickname);
	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());

	Instance->ResistUser(_id,_pw, nickname, this);
}

void UWIdget_Login::LoginSuccess()
{
	//UE_LOG(LogTemp, Log, TEXT("Login Success"));
}

void UWIdget_Login::LoginFail()
{
	//UE_LOG(LogTemp, Log, TEXT("Login Fail"));
}

void UWIdget_Login::SetCurrentWidget(AActor* Widget)
{
	UUserWidget* InputWidget = Cast<UUserWidget>(Widget);
	if(InputWidget) Current_Widget = InputWidget;
}

void UWIdget_Login::SetSoonDestroy()
{
	FTimerHandle FTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this,  &UWIdget_Login::SetDestroy, 0.8f, false );
	//World->GetTimerManager().SetTimer(FiringTimer, this, &AOnline_RPGCharacter::StopFire, FireRate, false);
}

void UWIdget_Login::SetDestroy()
{
	RemoveFromParent();
}

void UWIdget_Login::DoLogout()
{
	//UE_LOG(LogTemp, Log, TEXT("DoLogout init "));
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	ALoginController* Controller =  Cast<ALoginController>(PlayerController);
	if(Controller)
	{
		if(!Controller->Current_Location.IsNearlyZero())
		{
			UNetwork_Manager_R* Network_Manager_R = Cast<UNetwork_Manager_R>(GetGameInstance());
			Network_Manager_R->SetSpawnData(Controller->Current_Location, Controller->Current_Rotatation, Controller->Login_ID);

			//위치 저장하고 꺼야하니 calback 으로 어플리케이션 종료
			return;
		}

		
	}

	if(PlayerController)
	{
		UKismetSystemLibrary::QuitGame(
			this, // WorldContextObject
			PlayerController, // Optional: A player controller to specify which player is quitting.
			EQuitPreference::Quit, // How the game should quit. EQuitPreference::Quit is typical.
			false // Ignore unsaved changes in the editor. Only relevant in the editor.
		);
	}
}


// void UWIdget_Login::LoginClick()
// {
// 	//UE_LOG(LogTemp, Log, TEXT("Login CLick!!!!"));
// 	UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());
// 	
// 	
// }
