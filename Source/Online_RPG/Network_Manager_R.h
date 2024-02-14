// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Http.h"



#include "Network_Manager_R.generated.h"

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
	void SelectUser(const FString& Username, const FString& Password);
	void InsertUser(const FString& Username, const FString& Password);
	void OnInsertUserResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
private:
	FSocket* Socket;
	// 기타 필요한 변수 및 함수들
};
