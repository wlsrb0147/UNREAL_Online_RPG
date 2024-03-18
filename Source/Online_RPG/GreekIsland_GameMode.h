// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GreekIsland_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class ONLINE_RPG_API AGreekIsland_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	int32 Unique_Player_Idx = 0;
	virtual void Logout(AController* Exiting) override;
	
		
};
