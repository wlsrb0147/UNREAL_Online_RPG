// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Http.h"
#include "MovieSceneSequencePlaybackSettings.h"



#include "Network_Manager_R.generated.h"

class AItemManager;
class ASound_Manager_R;
struct FMovieSceneSequencePlaybackSettings;
class UWIdget_Login;
/**
 * 
 */
UENUM(BlueprintType)
enum class SOUND_TYPE:uint8
{
	Btn_Click,
	Btn_GameStart,
	Btn_Click_Fail,
	BGM_Login,
	BGM_Ingame,
	Walk,
	BGM_Boss,
	Shoot_Sound_queue,
	Fire_Sound,
	Explosion_Sound,
	Walk_Grass,
	Walk_Water,
	Kraken_Attack,
	Kraken_Sound
	
};


UCLASS()
class ONLINE_RPG_API UNetwork_Manager_R : public UGameInstance
{
	GENERATED_BODY()


	

public:
	UNetwork_Manager_R();

	UPROPERTY(EditAnywhere,Category="DataBase")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASound_Manager_R> Sound_Class;
	ASound_Manager_R* Sound_Instance ;
	ASound_Manager_R* Get_Sound_Instance();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemManager> ItemManagerInstance_Class;
	AItemManager* ItemManagerInstance;
	AItemManager* GetItemManager();

	//에셋 매니저 관련
	UFUNCTION(BlueprintCallable)
	void LoadStartAsset();
	
	
	
	UFUNCTION(BlueprintCallable)
	void Sound_Play(SOUND_TYPE Sound_Type, int32 Audio_idx, FVector Location, FRotator Rotator, APawn* MyPawn = nullptr);

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

	UPROPERTY(EditAnywhere)
	UUserWidget* Player_Widget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> Player_Widget_Class;

	UPROPERTY(EditAnywhere)
	UUserWidget* Respawn_Widget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> Respawn_Widget_Class;


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
	//Quest
	int8 IsQuestAccept;

	//======================================로그인 후 관련 함수======================================
	void GetSpawnData();
	void GetSpawnData_CallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void Spawn_Init();
	void SetSpawnData(FVector _Location, FRotator _Rotation, FString _Login_ID, bool bIsLogout = true);
	void Game_Save();
	void SetSpawnData_Callback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void UpdateSpawnData();
	void InsertSpawnData();
	void Callback_ForceExit(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	FString DocumentId;

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
	
	TArray<TSharedPtr<FJsonValue>> ValuesArray;
	uint64 MoneyFromServer = 0;
	double CurrentHealthFromServer = 100;
	double MaxHealthFromServer = 100;
	
	bool CreateSession(ULocalPlayer* Player, int32 NumPublicConnections, bool bIsLAN);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	/*void FindSessions(APlayerController* PlayerController, bool bIsLAN, int32 MaxSearchResults);
	void OnFindSessionsComplete(bool bWasSuccessful);*/
	
	
private:
	//FSocket* Socket;
	// 기타 필요한 변수 및 함수들
	//TSharedPtr<FOnlineSessionSearch> SessionSearch;
};


