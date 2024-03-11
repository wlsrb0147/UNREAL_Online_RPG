// Fill out your copyright notice in the Description page of Project Settings.


#include "CMSpawnManager.h"
#include "TimerManager.h"
// Sets default values
ACMSpawnManager* ACMSpawnManager::Instance = nullptr;

ACMSpawnManager::ACMSpawnManager()
{
    // 생성자에서 인스턴스 초기화
    Instance = this;
    PrimaryActorTick.bCanEverTick = true;

}

AActor* ACMSpawnManager::SpawnActor(UClass* ActorClass, const FVector& Location, const FRotator& Rotation)
{
    AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, Location, Rotation);
    return NewActor;
}


void ACMSpawnManager::DestroyActor(AActor* Actor)
{
    if (!Actor)
    {
        UE_LOG(LogTemp, Error, TEXT("Actor is null!"));
        return;
    }
    // 엑터 소멸
    Actor->Destroy();

    // 배열에서 제거
    SpawnedActors.Remove(Actor);
}

ACMSpawnManager* ACMSpawnManager::GetInstance()
{
    return Instance;
}


