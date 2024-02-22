// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDog.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "EnemyProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"

// Sets default values
AEnemyDog::AEnemyDog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyDog::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	RangeCheck();
}

// Called every frame
void AEnemyDog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyDog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyDog::Attack_Implementation()
{
	SpawnProjectile();
}



float AEnemyDog::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	AEnemyAIController* OwnerController = Cast<AEnemyAIController>(this->GetController());
	OwnerController->SetPlayer(Cast<APawn>(DamageCauser));
	if (IsDead) return ActualDamage;

	if(Health-ActualDamage<= 0)
	{
		Dead();
		return ActualDamage;
	}

	Health -= ActualDamage;
	UE_LOG(LogTemp, Display, TEXT("Actor took damage: %f Heath = %f"), ActualDamage, Health);
	return ActualDamage;
}


void AEnemyDog::SpawnDebugSphere(FVector Location, float Radius)
{
	DrawDebugSphere(
		GetWorld(), 
		Location,
		Radius, 
		20, 
		FColor::Red, 
		false, 
		2, // 스피어를 유지할 시간(초)
		0, 
		1 
	);
}

void AEnemyDog::SpawnProjectile()
{
	TSubclassOf<AEnemyProjectile> ActorClassToSpawn = AEnemyProjectile::StaticClass();

	FVector SpawnLocation = this->GetActorLocation();
	FRotator SpawnRotation = this->GetActorRotation();

	AActor * Projectile = GetWorld()->SpawnActor<AEnemyProjectile>(AttackProjectile, SpawnLocation, SpawnRotation);

}

bool AEnemyDog::RangeCheck()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	float TargetDis = FVector::Dist(this->GetActorLocation(), PlayerPawn->GetActorLocation());

	if(AttackRange>=TargetDis)
	{
		return true;
	}
	return false;

}
void AEnemyDog::Dead()
{
	if(IsDead) return;
	Health = 0;
	IsDead = true;
	FRotator MyRotator(0.0f, 0.0f, 190.0f);
	this->AddActorLocalRotation(MyRotator);
	AEnemyAIController* OwnerController = Cast<AEnemyAIController>(this->GetController());
	if (OwnerController)
	{
		OwnerController->Dead();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyAIController Is Nullptr"));
	}
}
