
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
		
		QQ->HandleAttack();
		UE_LOG(LogTemp, Warning, TEXT(" Attking1"));
	}

	//OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("TargetLastLocation"));

	return EBTNodeResult::Succeeded;
}