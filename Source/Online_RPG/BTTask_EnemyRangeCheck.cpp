
#include "BTTask_EnemyRangeCheck.h"
#include "EnemyDog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_EnemyRangeCheck::UBTTask_EnemyRangeCheck()
{
	NodeName = "EnemyRangeCheck";
}

EBTNodeResult::Type UBTTask_EnemyRangeCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() != nullptr)
	{
		AEnemyDog* QQ = Cast<AEnemyDog>(OwnerComp.GetAIOwner()->GetPawn());
		if(QQ->RangeCheck(1))
		{	
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttackRange1"), true);
		}
		else 
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttackRange1"), false);
		}
		if (QQ->RangeCheck(2))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttackRange2"), true);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttackRange2"), false);
		}
	}
	return EBTNodeResult::Succeeded;
}

