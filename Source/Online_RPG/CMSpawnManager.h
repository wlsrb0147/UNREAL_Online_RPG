// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMSpawnManager.generated.h"

UCLASS()
class ONLINE_RPG_API ACMSpawnManager : public AActor
{
    GENERATED_BODY()

public:
    ACMSpawnManager();

    // 스폰 함수
    AActor* SpawnActor(UClass* ActorClass, const FVector& Location, const FRotator& Rotation);

    // 소멸 함수
    void DestroyActor(AActor* Actor);

    // 싱글톤 인스턴스 반환
    static ACMSpawnManager* GetInstance();

    


private:
    // 생성된 엑터를 관리할 배열
    UPROPERTY(EditAnywhere)
    TArray<AActor*> SpawnedActors;
    UPROPERTY(EditAnywhere)
    TSubclassOf<class AEnemyDog> EnemyDog;
    // 싱글톤 인스턴스
    static ACMSpawnManager* Instance;

};
