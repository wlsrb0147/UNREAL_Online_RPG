// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PostLoadAsset.generated.h"

/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UPostLoadAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	//=============수동 로드 SOUND=============== 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AsyncLoadSound")
	USoundBase* Ingame_Sound_queue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AsyncLoadSound")
	USoundBase* Walk_Sound_queue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AsyncLoadSound")
	USoundBase* Walk_Sound_Water_queue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AsyncLoadSound")
	USoundBase* Walk_Sound_Grass_queue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AsyncLoadSound")
	USoundBase* Boss_BGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AsyncLoadSound")
	USoundBase* Shoot_Sound_queue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AsyncLoadSound")
	USoundBase* Fire_Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AsyncLoadSound")
	USoundBase* Explosion_Sound;
	//=============== =============== =============== 
};
