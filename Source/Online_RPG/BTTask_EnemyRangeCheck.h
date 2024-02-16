// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EnemyRangeCheck.generated.h"

/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UBTTask_EnemyRangeCheck : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UBTTask_EnemyRangeCheck();
};
