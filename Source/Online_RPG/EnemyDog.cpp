// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDog.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
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

void AEnemyDog::Attack()
{
	if(Attack1Particle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, Attack1Particle, this->GetActorLocation(), this->GetActorRotation());
	}
	UE_LOG(LogTemp, Display, TEXT("Enemy Attack "));
}
float AEnemyDog::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 기본 동작으로 데미지를 받음
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 여기에 데미지를 처리하고 추가 작업을 수행하는 코드를 추가할 수 있음
	UE_LOG(LogTemp, Warning, TEXT("Actor took damage: %f"), ActualDamage);

	return ActualDamage;
}