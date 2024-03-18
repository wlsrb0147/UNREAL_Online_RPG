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
	
	FString Login_ID;

	UFUNCTION(BlueprintPure)
	FString Get_Login_ID() { return Login_ID; }

	UFUNCTION(NetMulticast, Reliable)
	void SetLoginID(const FString& _Login_ID);
	
	virtual void PostNetInit() override;
	virtual void OnPossess(APawn* InPawn) override;

	FVector Current_Location = FVector(0,0,0);
	FRotator Current_Rotatation = FRotator(0,0,0);
	
	

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ChangePawn(int PlayerIdx, FVector _SpawnLocation, FRotator _SpawnRotation);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> SpawnPawn;


	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;
};
