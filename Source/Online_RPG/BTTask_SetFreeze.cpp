// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetFreeze.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyDog.h"
#include "AIController.h"
UBTTask_SetFreeze::UBTTask_SetFreeze()
{
	NodeName = "Set Freeze = false";
}

EBTNodeResult::Type UBTTask_SetFreeze::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() != nullptr)
	{
		AEnemyDog* QQ = Cast<AEnemyDog>(OwnerComp.GetAIOwner()->GetPawn());
		QQ->HitOff();
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("Freeze", false);

	return EBTNodeResult::Succeeded;
}