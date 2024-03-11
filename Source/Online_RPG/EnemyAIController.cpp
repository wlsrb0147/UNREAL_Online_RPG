// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "PlayerCharacter.h"
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
	if(AttackPawn)
	{
		if (LineOfSightTo(AttackPawn))
		{
			SetFocus(AttackPawn);
			GetBlackboardComponent()->SetValueAsVector(TEXT("TargetCurrentLocation"), AttackPawn->GetActorLocation());
			GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLastLocation"), AttackPawn->GetActorLocation());
		}
		else
		{
			ClearFocus(EAIFocusPriority::Gameplay);
			Blackboard->ClearValue(TEXT("TargetCurrentLocation"));
			//StopMovement();
		}
	}
}
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
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

void AEnemyAIController::SetPlayer(APawn* _pawn)
{
	AttackPawn = _pawn;
}
void AEnemyAIController::PlayerRangeCheck()
{
	// 플레이어를 저장할 배열
	TArray<AActor*> Players;

	// 현재 맵에서 모든 플레이어 찾기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Players);

	// 각 플레이어와의 거리 체크
	for (AActor* PlayerActor : Players)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerActor);
		if (PlayerCharacter)
		{
			FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			FVector AILocation = GetPawn()->GetActorLocation();
			float DistanceToPlayer = FVector::Distance(PlayerLocation, AILocation);

			// 특정 거리 내에 플레이어가 있는 경우
			if (DistanceToPlayer <= ChaseRange)
			{
				// 원하는 동작을 수행
			}
		}
	}
}
