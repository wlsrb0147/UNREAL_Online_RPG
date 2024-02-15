// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Projectile_dm.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	//마우스 이동시 캐릭터를 회전 시키지 않음
	bUseControllerRotationYaw = false;


	//플레이어 체력 초기화
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//발사체 클래스 초기화
	ProjectileClass = AProjectile_dm::StaticClass();
	//발사 속도 초기화
	FireRate = 0.25f;
	bIsFiringWeapon = false;

	//죽음 상태 초기화
	bIsDead = false;

	//Shoot 상태 초기화
	bIsShoot = false;

}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//Dead 애니메이션 테스트 코드
	/*FTimerHandle TestTimerHandle;
	GetWorldTimerManager().SetTimer(TestTimerHandle, this, &APlayerCharacter::SetIsDead, 0.1f);*/

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (GetMovementComponent()->IsFalling()) {
		UE_LOG(LogTemp, Warning, TEXT("IsFalling : True"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("IsFalling : False"));
	}*/

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (Input != nullptr)
	{
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);


		// 발사체 발사 처리
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::StartFire);
	}

}

void APlayerCharacter::Move(const FInputActionInstance& Instance)
{
	FVector2D MovementVector = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		//UE_LOG(LogTemp, Warning, TEXT("XXXXX : %f, YYYYY : %f"), MovementVector.X, MovementVector.Y);
	}
}

void APlayerCharacter::Look(const FInputActionInstance& Instance)
{
	FVector2D LookVector = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(-LookVector.X);
		AddControllerPitchInput(LookVector.Y);

		//UE_LOG(LogTemp, Warning, TEXT("XXXXX : %f, YYYYY : %f"), LookVector.X, LookVector.Y);
	}
}



// 리플리케이트된 프로퍼티
void APlayerCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//현재 체력 리플리케이트
	DOREPLIFETIME(APlayerCharacter, CurrentHealth);

	//현재 죽음 상태 리플리케이트
	DOREPLIFETIME(APlayerCharacter, bIsDead);
}



void APlayerCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);

		//화면 출력
		FString HealthMessage = FString::Printf(TEXT("CurrentHealth : %f"), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, HealthMessage);

		OnHealthUpdate();
	}
}

void APlayerCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
	//함수
	//함수
	//함수

}

void APlayerCharacter::OnHealthUpdate()
{
	//클라이언트 전용 함수 기능
	if (IsLocallyControlled())
	{
		//FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			//FString deathMessage = FString::Printf(TEXT("You have been killed."));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);


		}
	}

	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority)
	{
		//FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0) {
			SetIsDead(true);
		}
	}

	//모든 머신에서 실행되는 함수
	/*
		여기에 대미지 또는 사망의 결과로 발생하는 특별 함수 기능 배치
	*/
}

void APlayerCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsShoot = true; 
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &APlayerCharacter::StopFire, FireRate, false);
		HandleFire();
	}
}

void APlayerCharacter::StopFire()
{
	bIsShoot = false;
	bIsFiringWeapon = false;
}

void APlayerCharacter::HandleFire_Implementation()
{
	//Projectile 스폰
	/*FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	AProjectile_dm* spawnedProjectile = GetWorld()->SpawnActor<AProjectile_dm>(spawnLocation, spawnRotation, spawnParameters);*/


	CMAttack();
}

void APlayerCharacter::SetIsShoot(bool IsShoot)
{
	bIsShoot = IsShoot;
}

void APlayerCharacter::SetIsDead(bool IsDead)
{
	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority)
	{
		bIsDead = IsDead;
		OnIsDeadUpdate();
	}
}

void APlayerCharacter::SetIsDead()
{
	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority)
	{
		bIsDead = true;
		OnIsDeadUpdate();
	}
}

void APlayerCharacter::OnRep_IsDead()
{
	OnIsDeadUpdate();
}

void APlayerCharacter::OnIsDeadUpdate()
{
	//클라이언트 전용 함수 기능
	if (IsLocallyControlled())
	{
		if (bIsDead)
		{
			//...

		}
	}

	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority)
	{
		if (bIsDead)
		{
			//...

		}
	}

	//모든 머신에서 실행되는 함수
	/*
		여기에 대미지 또는 사망의 결과로 발생하는 특별 함수 기능 배치
	*/
}


float APlayerCharacter::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);

	//화면 출력
	FString TakeDamageMessage = FString::Printf(TEXT("TakeDamage Damage : %f"), DamageTaken);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TakeDamageMessage);
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage Damage : %f"), DamageTaken);

	return damageApplied;
}


void APlayerCharacter::CMAttack()
{


	APawn* OwnerPawn = Cast<APawn>(this);
	if (OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;

	FVector Location;
	FRotator Rotation;

	OwnerController->GetPlayerViewPoint(Location, Rotation);

	//ECC_GameTraceChannel1

	FVector End = Location + Rotation.Vector() * CMAttackRange;
	FHitResult Hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, params);
	if (bSuccess)
	{
		FVector ShotDirection = -Rotation.Vector();

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			SpawnDebugSphere(Hit.ImpactPoint, 30);

			FPointDamageEvent DamageEvent(CMAttackDamage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(CMAttackDamage, DamageEvent, OwnerController, this);
			
			

			//화면 출력
			FString AttackMessage = FString::Printf(TEXT("Attack Damage : %f"), CMAttackDamage);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, AttackMessage);
			UE_LOG(LogTemp, Warning, TEXT("Attack Damage : %f"), CMAttackDamage);

			FString HitActorMessage = FString::Printf(TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, HitActorMessage);
			UE_LOG(LogTemp, Warning, TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
		}

	}

}
void APlayerCharacter::SpawnDebugSphere(FVector Location, float Radius)
{

	DrawDebugSphere(
		GetWorld(),
		Location,
		Radius,
		20,
		FColor::Green,
		false,
		2, // 스피어를 유지할 시간(초)
		0,
		1
	);
}
