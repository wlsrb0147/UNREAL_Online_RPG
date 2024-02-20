// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
AEnemyAIController::AEnemyAIController()
{

}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsDead)
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(LineOfSightTo(PlayerPawn))
	{
		SetFocus(PlayerPawn);
		GetBlackboardComponent()->SetValueAsVector(TEXT("TargetCurrentLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLastLocation"), PlayerPawn->GetActorLocation());
	}
	else
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		Blackboard->ClearValue(TEXT("TargetCurrentLocation"));
		//StopMovement();
	}
}
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		/*GetBlackboardComponent()->SetValueAsVector(TEXT("TargetCurrentLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLastLocation"), PlayerPawn->GetActorLocation());*/
		GetBlackboardComponent()->SetValueAsVector(TEXT("SpawnLocation"), GetPawn()->GetActorLocation());
	}

	
}

void AEnemyAIController::Dead()
{
	UBlackboardComponent* MyBlackboard = FindComponentByClass<UBlackboardComponent>();
	if (MyBlackboard)
	{
		IsDead = true;
		MyBlackboard->SetValueAsBool(TEXT("IsDead"), IsDead);
		UE_LOG(LogTemp,Display,TEXT("AIController IsDead True"))
		StopBehaviorTree();
	}
	
}



void AEnemyAIController::StopBehaviorTree()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(this->GetBrainComponent());
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void AEnemyAIController::StartBehaviorTree()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(this->GetBrainComponent());
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StartTree(*this->AIBehavior, EBTExecutionMode::Looped);
	}
}