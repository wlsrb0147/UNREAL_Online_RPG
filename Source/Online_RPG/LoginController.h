// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LoginController.generated.h"

/**
 * 
 */
UCLASS()
class ONLINE_RPG_API ALoginController : public APlayerController
{
	GENERATED_BODY()
public:

	//서버가 컨트롤러 만들어줄때 처리
	ALoginController();

	UPROPERTY(Replicated)
	int32 INDEX_OF_PLAYER_CONTROLLER;

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ChangePawn(int PlayerIdx);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> SpawnPawn;

	
protected:
	virtual void BeginPlay() override;
};
