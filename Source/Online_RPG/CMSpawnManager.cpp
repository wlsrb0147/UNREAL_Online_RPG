// Fill out your copyright notice in the Description page of Project Settings.


#include "CMSpawnManager.h"
#include "TimerManager.h"
#include "EnemyDog.h"
#include "Net/UnrealNetwork.h"
#include "EnemyAIController.h"

// Sets default values

ACMSpawnManager::ACMSpawnManager()
{
    // 액터의 기본 속성 초기화
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

}

void ACMSpawnManager::SpawnEnemyDog(FVector Location)
{
    if (GetWorld())
    {

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        UE_LOG(LogTemp, Warning, TEXT("1"));
        // EnemyDog 액터 스폰
        AEnemyDog* SpawnedActor = GetWorld()->SpawnActor<AEnemyDog>(ActorToSpawn, Location, FRotator::ZeroRotator, SpawnParams);
        //SpawnedActor->BeginPlay();

        if (SpawnedActor != nullptr)
        {
            // AI Controller 생성 및 할당
            AEnemyAIController* NewAIController = GetWorld()->SpawnActor<AEnemyAIController>(EnemyAICon , FVector::ZeroVector, FRotator::ZeroRotator);
            if (NewAIController != nullptr)
            {
                NewAIController->Possess(SpawnedActor);
            }
        }

        // GetWorld()->GetTimerManager()를 사용하여 타이머 스케줄링
        //GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, RespawnTime, false);
    }
}
void ACMSpawnManager::SetSpawnEnemyDog(FVector Location, float RespawnTime)
{
    // FTimerHandle 객체 생성
    FTimerHandle TimerHandle;

    // FTimerDelegate 정의 및 SpawnActualEnemyDog 함수와 바인딩
    FTimerDelegate TimerDel;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Location]()
        {
            this->SpawnEnemyDog(Location);
        }, RespawnTime, false);
}
void ACMSpawnManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // ActorToSpawn 프로퍼티 리플리케이션 등록
    DOREPLIFETIME(ACMSpawnManager, ActorToSpawn);
}