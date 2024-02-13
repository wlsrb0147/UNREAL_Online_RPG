//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "EnemyAIController.h"
//#include "Kismet/GameplayStatics.h"
//#include "BehaviorTree/BlackboardComponent.h"
//AEnemyAIController::AEnemyAIController()
//{
//	;
//}
//
//void AEnemyAIController::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
//	if(LineOfSightTo(PlayerPawn))
//	{
//		SetFocus(PlayerPawn);
//		GetBlackboardComponent()->SetValueAsVector(TEXT("TargetCurrentLocation"), PlayerPawn->GetActorLocation());
//		GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLastLocation"), PlayerPawn->GetActorLocation());
//	}
//	else
//	{
//		ClearFocus(EAIFocusPriority::Gameplay);
//		Blackboard->ClearValue(TEXT("TargetCurrentLocation"));
//		//StopMovement();
//	}
//
//}
//void AEnemyAIController::BeginPlay()
//{
//	Super::BeginPlay();
//	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
//
//	if(AIBehavior != nullptr)
//	{
//		RunBehaviorTree(AIBehavior);
//		/*GetBlackboardComponent()->SetValueAsVector(TEXT("TargetCurrentLocation"), PlayerPawn->GetActorLocation());
//		GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLastLocation"), PlayerPawn->GetActorLocation());*/
//		GetBlackboardComponent()->SetValueAsVector(TEXT("SpawnLocation"), GetPawn()->GetActorLocation());
//	}
//
//	
//}