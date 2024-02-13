// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyDog.h"
#include "AIController.h"


UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
	NodeName = "Clear Blackboard Value";
}

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if(OwnerComp.GetAIOwner() != nullptr)
	{
		AEnemyDog* QQ = Cast<AEnemyDog>(OwnerComp.GetAIOwner()->GetPawn());
	}
	OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("TargetLastLocation"));

	return EBTNodeResult::Succeeded;
}