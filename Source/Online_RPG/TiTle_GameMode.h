// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TiTle_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class ONLINE_RPG_API ATiTle_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UUserWidget* Login_Widget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> Login_Widget_CLASS;
	//class UUserWidget* Login_Widget;
	
};
