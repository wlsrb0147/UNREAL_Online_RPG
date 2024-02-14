// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDog.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"

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


	APawn* OwnerPawn = Cast<APawn>(this);
	if (OwnerPawn == nullptr) return;

	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;

	FVector Location = OwnerPawn->GetActorLocation();
	FRotator Rotation = OwnerPawn->GetActorRotation();

	//ECC_GameTraceChannel1

	FVector End = Location + Rotation.Vector() * MaxRange;

	FHitResult Hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	SpawnDebugSphere(End, 40);
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, params);
	if (bSuccess)
	{
		FVector ShotDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Attack1Particle, Hit.Location, ShotDirection.Rotation());
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage1, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage1, DamageEvent, OwnerController, this);
		}

	}
}
float AEnemyDog::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 기본 동작으로 데미지를 받음
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 여기에 데미지를 처리하고 추가 작업을 수행하는 코드를 추가할 수 있음
	UE_LOG(LogTemp, Warning, TEXT("Actor took damage: %f"), ActualDamage);
	Health = ActualDamage;
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