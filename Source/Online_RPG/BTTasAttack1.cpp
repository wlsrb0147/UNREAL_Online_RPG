// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTasAttack1.h"
#include "EnemyDog.h"
#include "AIController.h"

UBTTasAttack1::UBTTasAttack1()
{
	NodeName = "Attack";
}


EBTNodeResult::Type UBTTasAttack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() != nullptr)
	{
		AEnemyDog* QQ = Cast<AEnemyDog>(OwnerComp.GetAIOwner()->GetPawn());
		QQ->Attack();
	}
	//OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("TargetLastLocation"));

	return EBTNodeResult::Succeeded;
}