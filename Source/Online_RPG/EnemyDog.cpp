// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDog.h"
#include "EnemyProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
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

void AEnemyDog::Attack()
{
	SpawnActor22();

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

void AEnemyDog::SpawnActor22()
{

	// 스폰할 액터 클래스 정의
	TSubclassOf<AEnemyProjectile> ActorClassToSpawn = AEnemyProjectile::StaticClass();

	// 스폰 위치와 회전 정의
	FVector SpawnLocation = this->GetActorLocation(); // 스폰할 위치
	FRotator SpawnRotation = this->GetActorRotation(); // 스폰할 회전

	// 액터 스폰
	AActor * Projectile = GetWorld()->SpawnActor<AEnemyProjectile>(AmmoBlueprint, SpawnLocation, SpawnRotation);
		//SpawnActor<AEnemyProjectile>(Projectile, SpawnLocation, SpawnRotation);

	if (Projectile)
	{
		// 액터가 스폰되었을 때 수행할 작업 추가
		// 예: 액터에 대한 추가 설정, 이벤트 바인딩 등
	}
	else
	{
		// 액터가 스폰되지 않았을 때 처리할 내용
	}
}

bool AEnemyDog::RangeCheck()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	float TargetDis = FVector::Dist(this->GetActorLocation(), PlayerPawn->GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("Target Distance : %f"), TargetDis);

	if(AttackRange>=TargetDis)
	{
		return true;
	}
	return false;

}

//void AEnemyDog::FireProjectile()
//{
//	if (ProjectileClass)
//	{
//		
//		// 프로젝타일 생성
//		FActorSpawnParameters SpawnParams;
//		SpawnParams.Owner = this;
//		SpawnParams.Instigator = Instigator;
//		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f; // 캐릭터 위치에서 앞으로 100 유닛 이동한 위치
//		FRotator SpawnRotation = GetActorRotation(); // 캐릭터의 현재 회전값 사용
//		UProjectileMovementComponent* Projectile = GetWorld()->SpawnActor<UProjectileMovementComponent>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
//		
//		// 프로젝타일 초기화
//		if (Projectile)
//		{
//			// 발사 방향과 속도 설정
//			FVector LaunchDirection = GetActorForwardVector();
//			Projectile->FireInDirection(LaunchDirection);
//		}
//	}
//}