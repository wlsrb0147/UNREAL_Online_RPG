// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Http.h"
#include "MovieSceneSequencePlaybackSettings.h"


#include "Network_Manager_R.generated.h"

struct FMovieSceneSequencePlaybackSettings;
class UWIdget_Login;
/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UNetwork_Manager_R : public UGameInstance
{
	GENERATED_BODY()


	

public:
	UNetwork_Manager_R();

	

	// // 함수들을 선언
	// bool ConnectToServer(const FString& IP, int32 Port);
	// void SendMessageToServer(const FString& Message);
	// void ReceiveMessageFromServer();

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnResponseReceived_Join(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SelectUser(const FString& Username, const FString& Password, UUserWidget* InputWidget);
	void InsertUser(const FString& Username, const FString& Password, const FString& NickName);
	void ResistUser(const FString& Username, const FString& Password, const FString& NickName, UUserWidget* InputWidget);
	void OnInsertUserResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FString Join_ID;
	FString Join_PW;
	FString Join_NickName;

	UPROPERTY(EditAnywhere)
	UUserWidget* Current_Widget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> Fail_Widget;
	//UWIdget_Login* Fail_Widget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> Success_Widget;
	//UWIdget_Login* Success_Widget;

	void CreateWidget_OX(bool bIsSuccess);


	//======================================로그인 후 세팅 값======================================
	//ID
	FString Login_ID;

	//Spawn 위치
	FVector SpawnLocation;
	//Spawn 회전값
	FRotator SpawnRotator;
	//Gun
	FString GunName;
	//Sword
	FString SwordName;

	//======================================로그인 후 관련 함수======================================
	void GetSpawnData();
	void GetSpawnData_CallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void Spawn_Init();

	class ULevelSequence* Level_Sequence;
	class ULevelSequencePlayer* Player_Sequence;
	class ALevelSequenceActor* SequenceActor;
	FMovieSceneSequencePlaybackSettings MovieSceneSequencePlaybackSettings;

	//시퀀스 콜백 함수
	UFUNCTION()
	void OnSequenceFinished();

	//스폰 캐릭터 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> SpawnPawn;

	//스폰 요청
	UFUNCTION(Server, Reliable)
	void CallSpawn(int Player_Idx);
	
	
	
	
	
	
private:
	//FSocket* Socket;
	// 기타 필요한 변수 및 함수들
};
