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
#include "Sword.h"
#include "Gun.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CollisionShape.h"
#include "LoginController.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 리플리케이트
	bReplicates = true;

	//마우스 이동시 캐릭터를 회전 시키지 않음
	bUseControllerRotationYaw = false;


	//플레이어 체력 초기화
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//발사체 클래스 초기화
	ProjectileClass = AProjectile_dm::StaticClass();
	//발사 속도 초기화
	CoolTime = 0.11f;
	bIsAttacking = false;

	//죽음 상태 초기화
	bIsDead = false;

	//Shoot 상태 초기화
	bIsShoot = false;

	//UpperSlash 상태 초기화
	bIsUpperSlash = false;

	//bReplicates = true;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);
	UE_LOG(LogTemp, Log, TEXT("========================="));

	UE_LOG(LogTemp, Log, TEXT("Controller is POSSESSED!!!!!!!!!!!!!!!!: %s "), *NewController->GetName());
	UE_LOG(LogTemp, Log, TEXT("========================="));
}

void APlayerCharacter::OnRep_Owner()
{
	FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);
	UE_LOG(LogTemp, Log, TEXT("========================="));
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		UE_LOG(LogTemp, Log, TEXT("OnRep_Owner!!!!!!!!!!!!!!!!:  %s"), *GetOwner()->GetActorNameOrLabel());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No Owner "));
	}
	Super::OnRep_Owner();





	OwnerActor = GetOwner();
	if (OwnerActor)
	{
		UE_LOG(LogTemp, Log, TEXT("OnRep_Owner!!!!!!!!!!!!!!!!:  %s"), *GetOwner()->GetActorNameOrLabel());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No Owner "));
	}
	UE_LOG(LogTemp, Log, TEXT("========================="));
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 현재 실행 환경이 서버인지 클라이언트인지 확인
	// FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	// UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);
	if (IsLocallyControlled())
	{
		UE_LOG(LogTemp, Display, TEXT("Locally controlled: %s"), *GetActorNameOrLabel());
	}
	else if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Display, TEXT("Server controlled: %s"), *GetActorNameOrLabel());
	}
	else if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		UE_LOG(LogTemp, Display, TEXT("Simulated proxy: %s"), *GetActorNameOrLabel());
	}
	else if (GetLocalRole() == ROLE_None)
	{
		UE_LOG(LogTemp, Display, TEXT("No network role: %s"), *GetActorNameOrLabel());
	}

	UE_LOG(LogTemp, Log, TEXT("========================= %s %s"), *GetName(), *GetActorNameOrLabel());

	if (GetController() != nullptr)
	{

		EnableInput(Cast<ALoginController>(GetController()));
		UE_LOG(LogTemp, Log, TEXT("Controller is assigned: %s %s"), *GetController()->GetName(), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is not assigned."));
	}




	UE_LOG(LogTemp, Log, TEXT("========================="));

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (SwordClass)
	{
		UE_LOG(LogTemp, Log, TEXT("칼 생성 했자나"));
		MySword = GetWorld()->SpawnActor<ASword>(SwordClass);
		MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket_r"));
		MySword->SetOwner(this);
	}

	if (GunClass)
	{
		UE_LOG(LogTemp, Log, TEXT("총 생성 했자나"));
		MyGun = GetWorld()->SpawnActor<AGun>(GunClass);
		MyGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket_l"));
		MyGun->SetOwner(this);
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


	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FVector End = Location + Rotation.Vector() * ShootAttackRange;

	DrawDebugLine(GetWorld(), Location, End, FColor::Cyan, false);
	DrawDebugBox(GetWorld(), End, FVector(10, ShootAttackWidth, ShootAttackHeight), Rotation.Quaternion(), FColor::Cyan, false);
	DrawDebugBox(GetWorld(), Location, FVector(10, ShootAttackWidth, ShootAttackHeight), Rotation.Quaternion(), FColor::Cyan, false);

	/*if (bIsAttacking) {
		UE_LOG(LogTemp, Display, TEXT("bIsAttacking TRUE"));
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("bIsAttacking FALSE"));
	}*/

	//SpawnDebugCapsule(Location, FVector(UpperSlashAttackRadius, UpperSlashAttackRadius, UpperSlashAttackHeight));
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent ..."));
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent22 ..."));

	//뉴인풋
	//UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//if (Input != nullptr)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent33 ..."));
	//	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	//	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	//	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	//	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

	//	//UpperSlash Bind
	//	Input->BindAction(UpperSlashAction, ETriggerEvent::Triggered, this, &APlayerCharacter::UpperSlash);
	//	// 발사체 발사 처리
	//	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::StartFire);
	//	Input->BindAction(FireDownAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartFire);
	//	Input->BindAction(FireUpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StopFire);
	//	//공격
	//	//Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartFire);
	//	PlayerInputComponent->BindAction(TEXT("Fire1"), IE_Pressed, this, &APlayerCharacter::StartFire);
	//	UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent44 ..."));
	//}

	//구 인풋
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	//PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &APlayerCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &APlayerCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("FireDownAction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("FireUpAction"), EInputEvent::IE_Released, this, &APlayerCharacter::StopFire);
	PlayerInputComponent->BindAction(TEXT("UpperSlash"), EInputEvent::IE_Pressed, this, &APlayerCharacter::UpperSlash);
	//Input->BindAction(FireDownAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartFire);
	//Input->BindAction(FireUpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StopFire);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
}

void APlayerCharacter::Move(const FInputActionInstance& Instance)
{
	//UE_LOG(LogTemp,Log,TEXT("Move ..."));
	//공격중에는 움직임 X
	if (bIsAttacking) return;

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

void APlayerCharacter::MoveForward(float AxisValue)
{
	if (bIsAttacking) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(FowardDirection, AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if (bIsAttacking) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightDirection, AxisValue);
}

void APlayerCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * 20.f * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * 20.f * GetWorld()->GetDeltaSeconds());
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

void APlayerCharacter::UpperSlash_Implementation()
{
	if (bIsAttacking) return;
	UE_LOG(LogTemp, Log, TEXT("Upper Slash init"));
	//FTimerHandle Handle;
	//GetWorldTimerManager().SetTimer(Handle, this, &APlayerCharacter::StopUpperSlash, UpperSlash_Rate, false);
	bIsUpperSlash = true;
	bIsAttacking = true;
}

void APlayerCharacter::StopUpperSlash_Implementation()
{
	bIsUpperSlash = false;
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &APlayerCharacter::AttackCoolTime, CoolTime, false);
	bIsAttacking = false;
}

bool APlayerCharacter::GetIsUpperSlash() const
{
	return bIsUpperSlash;
}

void APlayerCharacter::OnRep_IsUpperSlash()
{
}

void APlayerCharacter::HandleUpperSlash()
{
	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority) {
		UE_LOG(LogTemp, Display, TEXT("UpperSlashAttack!!!!!!!!!"));
		UpperSlashAttack();
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
	DOREPLIFETIME(APlayerCharacter, bIsUpperSlash);
	DOREPLIFETIME(APlayerCharacter, bIsShoot);
	DOREPLIFETIME(APlayerCharacter, bIsShootAnim);
	DOREPLIFETIME(APlayerCharacter, bIsAttacking);

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

void APlayerCharacter::StartFire_Implementation()
{

	if (bIsAttacking) return;
	UE_LOG(LogTemp, Log, TEXT("START RFIRE"));

	UseControllerRotationYaw_Toggle_Multi(true);

	//UE_LOG(LogTemp, Display, TEXT("??????????????"));
	bIsShoot = true;
	bIsAttacking = true;
	//bIsShootAnim = true;
	//UWorld* World = GetWorld();
	//World->GetTimerManager().SetTimer(FiringTimer, this, &APlayerCharacter::StopFire, FireRate, false);
	//HandleFire();

}

void APlayerCharacter::StopFire_Implementation()
{
	
	
	if (!bIsShoot) return;

	UseControllerRotationYaw_Toggle_Multi(false);
	UE_LOG(LogTemp, Display, TEXT("FALSE"));

	bIsShoot = false;
	//UWorld* World = GetWorld();
	//World->GetTimerManager().SetTimer(FiringTimer, this, &APlayerCharacter::AttackCoolTime, FireRate, false);
	bIsAttacking = false;
}

void APlayerCharacter::OnRep_IsAttacking()
{
	OnIsAttackingUpdate();
}

void APlayerCharacter::OnIsAttackingUpdate()
{

}

void APlayerCharacter::AttackCoolTime() {
	bIsAttacking = false;
}

void APlayerCharacter::HandleFire()
{
	//Projectile 스폰
	/*FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	AProjectile_dm* spawnedProjectile = GetWorld()->SpawnActor<AProjectile_dm>(spawnLocation, spawnRotation, spawnParameters);*/


	//CMAttack();
	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority) {
		UE_LOG(LogTemp, Display, TEXT("Attack!!!!!!!!!"));
		ShootAttack();
	}

}

void APlayerCharacter::SetIsShoot(bool IsShoot)
{
	bIsShoot = IsShoot;
}

void APlayerCharacter::SetIsShootAnim(bool IsShootAnim)
{
	bIsShootAnim = IsShootAnim;
}

void APlayerCharacter::OnRep_IsShootAnim()
{
	OnIsShootAnimUpdate();
}

void APlayerCharacter::OnIsShootAnimUpdate()
{
}

void APlayerCharacter::StartShootAnim()
{
	bIsShootAnim = true;
}

void APlayerCharacter::EndShootAnim()
{
	bIsShootAnim = false;
}

void APlayerCharacter::SpawnShootEffect_Implementation()
{
	USceneComponent* ShootEffectSpawnPoint = MyGun->EffectSpawnPoint;
	UGameplayStatics::SpawnEmitterAtLocation(this, ShootPaticles, ShootEffectSpawnPoint->GetComponentLocation(), ShootEffectSpawnPoint->GetComponentRotation(), FVector(50000.f));//FVector(ShootEffectScale));

}

void APlayerCharacter::StartAttack()
{
	if (!bIsAttackWeapon)
	{
		bIsAttack = true;
		bIsAttackWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(AttackTimer, this, &APlayerCharacter::StopAttack, AttackRate, false);
		HandleAttack();
	}
}

void APlayerCharacter::StopAttack()
{
	bIsAttack = false;
	bIsAttackWeapon = false;
}

void APlayerCharacter::HandleAttack_Implementation()
{

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
			UE_LOG(LogTemp, Display, TEXT("7777"));
		}
	}

	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority)
	{
		if (bIsDead)
		{
			//...
			UE_LOG(LogTemp, Display, TEXT("8888"));
		}
	}

	//모든 머신에서 실행되는 함수
	/*
		여기에 대미지 또는 사망의 결과로 발생하는 특별 함수 기능 배치
	*/
	if (bIsDead)
	{
		//중력 끔
		//GetCapsuleComponent()->SetEnableGravity(false);
		//콜리전 끔
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 죽음 상태에서는 물리 효과 제거
		GetCharacterMovement()->DisableMovement();
		//GetCapsuleComponent()->SetHiddenInGame(true);
		//SetActorTickEnabled(false);

	}

}

void APlayerCharacter::OnRep_IsShoot()
{
	OnIsShootUpdate();
}

void APlayerCharacter::OnIsShootUpdate()
{
	//클라이언트 전용 함수 기능
	if (IsLocallyControlled())
	{
		if (bIsShoot)
		{
			//...

		}
	}

	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority)
	{
		if (bIsShoot)
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

	FVector End = Location + Rotation.Vector() * ShootAttackRange;
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

			FPointDamageEvent DamageEvent(ShootAttackDamage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(ShootAttackDamage, DamageEvent, OwnerController, this);



			//화면 출력
			FString AttackMessage = FString::Printf(TEXT("Attack Damage : %f"), ShootAttackDamage);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, AttackMessage);
			UE_LOG(LogTemp, Warning, TEXT("Attack Damage : %f"), ShootAttackDamage);

			FString HitActorMessage = FString::Printf(TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, HitActorMessage);
			UE_LOG(LogTemp, Warning, TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
		}

	}

}

void APlayerCharacter::ShootAttack()
{

	//if (!HasAuthority()) return;
	//if(!HasLocalNetOwner()) return;
	/*
	FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);

	if(GetOwner())
	{
		UE_LOG(LogTemp,Log, TEXT(" Shoot 의 오우너 :  %s "), *GetOwner()->GetName())	;
	}

	if(GetNetOwner()){

		UE_LOG(LogTemp,Log, TEXT(" Shoot 의 넷 오우너 :  %s "), *GetNetOwner()->GetName())	;

	}*/
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	FCollisionShape AttackShape = FCollisionShape::MakeBox(FVector(10, ShootAttackWidth, ShootAttackHeight));

	//DrawDebugBox(GetWorld(), MyGun->EffectSpawnPoint->GetComponentLocation(), FVector(10, 50, 50), FColor::Cyan, false, 3, 0, 1);

	FVector End = Location + Rotation.Vector() * ShootAttackRange;
	FHitResult Hit;
	FCollisionQueryParams params;

	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	params.AddIgnoredActor(MyGun);

	USceneComponent* ShootEffectSpawnPoint = MyGun->EffectSpawnPoint;
	SpawnEmitterAtLocation_Multi(this, ShootPaticles, ShootEffectSpawnPoint->GetComponentLocation(), GetActorRotation(), FVector(ShootEffectScale));

	bool bSuccess = GetWorld()->SweepSingleByChannel(
		Hit,
		Location,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		AttackShape,
		params
	);

	if (bSuccess)
	{
		FVector ShotDirection = -Rotation.Vector();

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr && GetController()->LineOfSightTo(HitActor))
		{
			SpawnDebugSphere(Hit.ImpactPoint, 30);

			//피격 이펙트 생성
			//UGameplayStatics::SpawnEmitterAtLocation(this, ShootHitPaticles, Hit.ImpactPoint, FRotator::ZeroRotator, FVector(ShootHitEffectScale));
			SpawnEmitterAtLocation_Multi(this, ShootHitPaticles, Hit.ImpactPoint, FRotator::ZeroRotator, FVector(ShootHitEffectScale));

			FPointDamageEvent DamageEvent(ShootAttackDamage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(ShootAttackDamage, DamageEvent, GetController(), this);

			//화면 출력
			FString AttackMessage = FString::Printf(TEXT("Attack Damage : %f"), ShootAttackDamage);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, AttackMessage);
			UE_LOG(LogTemp, Warning, TEXT("Attack Damage : %f"), ShootAttackDamage);

			FString HitActorMessage = FString::Printf(TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, HitActorMessage);
			UE_LOG(LogTemp, Warning, TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
		}

		DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 3, 0, 5);
		DrawDebugBox(GetWorld(), End, FVector(10, ShootAttackWidth, ShootAttackHeight), Rotation.Quaternion(), FColor::Red, false, 3, 0, 5);

	}

}

void APlayerCharacter::UpperSlashAttack()
{

	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	//콜리전 크기
	FVector CollisionExtent = GetSimpleCollisionCylinderExtent();
	//UE_LOG(LogTemp, Warning, TEXT("CollisionExtent : %f, %f, %f"), CollisionExtent.X, CollisionExtent.Y, CollisionExtent.Z);

	FVector CapsuleSize = FVector(UpperSlashAttackRadius, UpperSlashAttackRadius, UpperSlashAttackHeight);

	//FCollisionShape AttackShape = FCollisionShape::MakeBox(FVector(10, ShootAttackWidth, ShootAttackHeight));
	FCollisionShape AttackShape = FCollisionShape::MakeCapsule(CapsuleSize);

	//DrawDebugBox(GetWorld(), MyGun->EffectSpawnPoint->GetComponentLocation(), FVector(10, 50, 50), FColor::Cyan, false, 3, 0, 1);

	FVector AttackLocation = (Location + FVector(0, 0, -CollisionExtent.Z + UpperSlashEffectOffsetZ)) + Rotation.Vector() * UpperSlashAttackDistance;
	TArray<FHitResult> Hits;
	FCollisionQueryParams params;

	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	params.AddIgnoredActor(MySword);

	//UParticleSystemComponent* ParticleSystemComponent = nullptr;
	SpawnEmitterAtLocation_Multi(this, UpperSlashPaticles, AttackLocation, GetActorRotation(), FVector(UpperSlashEffectScale), UpperSlashEffectDurationSec);
	SpawnEmitterAtLocation_SetTimer_Multi(this, UpperSlashFinishPaticles, UpperSlashEffectDurationSec, AttackLocation, GetActorRotation(), FVector(UpperSlashEffectFinishScale));

	bool bSuccess = GetWorld()->SweepMultiByChannel(
		Hits,
		AttackLocation,
		AttackLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		AttackShape,
		params
	);

	if (bSuccess)
	{
		FVector ShotDirection = -Rotation.Vector();

		for (int i = 0; i < Hits.Num(); i++) {
			AActor* HitActor = Hits[i].GetActor();
			if (HitActor != nullptr && GetController()->LineOfSightTo(HitActor))
			{
				//SpawnDebugSphere(Hit.ImpactPoint, 30);


				//피격 이펙트 생성
				//UGameplayStatics::SpawnEmitterAtLocation(this, ShootHitPaticles, Hit.ImpactPoint, FRotator::ZeroRotator, FVector(ShootHitEffectScale));

				FPointDamageEvent DamageEvent(UpperSlashAttackDamage, Hits[i], ShotDirection, nullptr);
				HitActor->TakeDamage(UpperSlashAttackDamage, DamageEvent, GetController(), this);

				//화면 출력
				FString AttackMessage = FString::Printf(TEXT("Attack Damage : %f"), ShootAttackDamage);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, AttackMessage);
				UE_LOG(LogTemp, Warning, TEXT("Attack Damage : %f"), ShootAttackDamage);

				FString HitActorMessage = FString::Printf(TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, HitActorMessage);
				UE_LOG(LogTemp, Warning, TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
			}
		}


		//DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 3, 0, 5);
		//DrawDebugBox(GetWorld(), End, FVector(10, ShootAttackWidth, ShootAttackHeight), Rotation.Quaternion(), FColor::Red, false, 3, 0, 5);
	}

	SpawnDebugCapsule(AttackLocation, CapsuleSize);

}

void APlayerCharacter::SpawnEmitterAtLocation_Multi_Implementation(const UObject* WorldContextObject, UParticleSystem* Particle, FVector Location, FRotator Rotation, FVector Scale, float DurationSec)
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnEmitterAtLocation"));
	//피격 이펙트 생성
	UParticleSystemComponent* ParticleSystemComponent;
	ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, Particle, Location, Rotation, Scale);


	//GetWorld()->GetTimerManager().SetTimer(FiringTimer, this, &APlayerCharacter::test, CoolTime, false);

	if (DurationSec > 0) {
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, ParticleSystemComponent]()
			{
				ParticleSystemComponent->DestroyComponent();
			}, DurationSec, false);
	}
}

void APlayerCharacter::SpawnEmitterAtLocation_SetTimer_Multi_Implementation(const UObject* WorldContextObject, UParticleSystem* Particle, float SetTime, FVector Location, FRotator Rotation, FVector Scale)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, WorldContextObject, Particle, Location, Rotation, Scale]()
		{
			UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, Particle, Location, Rotation, Scale);
		}, SetTime, false);
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

void APlayerCharacter::UseControllerRotationYaw_Toggle_Multi_Implementation(bool Val)
{
	bUseControllerRotationYaw = Val;
}

void APlayerCharacter::SpawnDebugCapsule(FVector Location, FVector CapsuleSize, FColor Color) {
	UE_LOG(LogTemp, Warning, TEXT("DrawDebugCapsule"));
	DrawDebugCapsule(
		GetWorld(),
		Location,
		CapsuleSize.Z / 2,
		CapsuleSize.X,
		FQuat::Identity,
		Color,
		false,
		2,
		0,
		1
	);
}
