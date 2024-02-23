// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ONLINE_RPG_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	AEnemyAIController();
protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Dead();
	void SetPlayer(APawn* _pawn);

	void PlayerRangeCheck();

private:
	UPROPERTY(EditAnywhere)
	float ChaseRange = 1000;


	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	void StopBehaviorTree();

	void StartBehaviorTree();

	UPROPERTY(VisibleAnywhere)
	bool IsDead = false;

	APawn* AttackPawn;

};
