// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMSpawnManager.generated.h"
class AEnemyDog;
class AEnemyAIController;
UCLASS()
class ONLINE_RPG_API ACMSpawnManager : public AActor
{
    GENERATED_BODY()

public:
    ACMSpawnManager();

    void SpawnEnemyDog(FVector Location);
    void SetSpawnEnemyDog(FVector Location, float RespawnTime);
    UPROPERTY(EditAnywhere)
    TSubclassOf<class AEnemyAIController> EnemyAICon;
    UPROPERTY(EditAnywhere, Replicated)
    TSubclassOf<class AEnemyDog> ActorToSpawn;
protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
