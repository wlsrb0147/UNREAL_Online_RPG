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
#include "InventoryComponent.h"
#include "InventoryHUD.h"
#include "InventoryPanel.h"
#include "ItemInteractionInterface.h"
#include "LoginController.h"
#include "Network_Manager_R.h"
#include "PickUpItem.h"
#include "Particles/ParticleSystemComponent.h"
#include <cmath>
#include "RED_Projectile.h"

#include "AItemManager.h"
#include "NPCConversation.h"
#include "SpawnActor.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 인벤토리 영역 인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역 ////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void APlayerCharacter::CheckInteraction()
{
	if (!IsLocallyControlled()) return;
	FVector TraceStart{ GetPawnViewLocation() };
	FVector TraceEnd{ TraceStart + GetViewRotation().Vector() * InteractionDistance };

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, -1, 0, 2);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(150.0);

	//FCollisionQueryParams
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	//DrawDebugSphere(
	//	GetWorld(),
	//	TraceEnd,
	//	Sphere.GetSphereRadius(),
	//	6,  // 스피어의 세그먼트 수
	//	FColor::Red,
	//	false  // true로 설정하면 지속적으로 그려집니다.
	//);


	if (!GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		Sphere,
		QueryParams)
		)
	{
		FoundNoInteract();
		return;
	}

	

		if (!HitResult.GetActor()->GetClass()->ImplementsInterface(UItemInteractionInterface::StaticClass())) return;

	if (HitResult.GetActor() != InteractionData.CurrentInteracting)
	{
		FoundInteract(HitResult.GetActor());
		return;
	}

	if (HitResult.GetActor() == InteractionData.CurrentInteracting)
	{
		return;
	}
}



void APlayerCharacter::FoundNoInteract()
{
	if (InteractionData.CurrentInteracting)
	{
		if (IsValid(InteractionTarget.GetObject()))
		{
			InteractionTarget->EndFocus();
		}

		HUD->CloseInteractionWidget();
		HUD->CloseConversationWidget();
		InteractionData.CurrentInteracting = nullptr;
		InteractionTarget = nullptr;
	}
}

void APlayerCharacter::FoundInteract(AActor* NewInteract)
{

	if (InteractionData.CurrentInteracting)
	{
		InteractionTarget = InteractionData.CurrentInteracting;
		InteractionTarget->EndFocus();
	}

	InteractionData.CurrentInteracting = NewInteract;
	InteractionTarget = NewInteract;
	
	HUD->UpdateInteractionWidget(&InteractionTarget->InteractionData);
	InteractionTarget->BeginFocus();
}

void APlayerCharacter::BeginInteract()
{
	//UE_LOG(LogTemp, Display, TEXT(" blue111 %d"), PlayerInventory->GetInventoryCapacity())
		CheckInteraction();

	if (!InteractionData.CurrentInteracting) return;

	if (!IsValid(InteractionTarget.GetObject())) return;

	InteractionTarget->BeginInteract();
	Interact();
}

void APlayerCharacter::Interact()
{

	if (!IsValid(InteractionTarget.GetObject())) return;
	
	if (InteractionTarget->InteractionData.InteractionType == EInteractionType::NPC)
	{
		HUD->CloseInteractionWidget();
		HUD->OpenConversationWidget(InteractionTarget->GetFBum());
		return;
	}

	APickUpItem* Tem = Cast<APickUpItem>(InteractionTarget.GetObject());
	Tem->SetOwner(this);
	
	RPC_Item_Owner(Tem, this);

	FoundNoInteract();

}

void APlayerCharacter::EndInteract()
{
	if (!IsValid(InteractionTarget.GetObject())) return;

	InteractionTarget->EndInteract();
}

void APlayerCharacter::OpenInventory()
{
	HUD->ToggleInventoryWidget();
}

void APlayerCharacter::UpdateInteractionWidget() const
{
	if (IsValid(InteractionTarget.GetObject()))
	{
		HUD->UpdateInteractionWidget(&InteractionTarget->InteractionData);
	}
}

void APlayerCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (!PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		return;
	}


	const FVector SpawnLocation{ GetActorLocation() + GetActorForwardVector() * 50.0f };
	const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
	const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);
	
	int32 Key = FCString::Atoi(*ItemToDrop->BaseItemID.ToString());
	RPC_Drop_Item(Key, RemovedQuantity);
//
////	ItemManager& ItemManagerInstance = ItemManager::Get();
//	
//	ItemManagerInstance->SpawnItem(this,ItemToDrop,SpawnTransform,RemovedQuantity);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 인벤토리 영역 인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역 ////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// Sets default values
APlayerCharacter::APlayerCharacter() : HUD(nullptr)
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
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 인벤토리 영역 인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역 ////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("인벤토리"));
	PlayerInventory->SetInventoryCapacity(24);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 인벤토리 영역 인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역 ////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	////UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);
	////UE_LOG(LogTemp, Log, TEXT("========================="));

	////UE_LOG(LogTemp, Log, TEXT("Controller is POSSESSED!!!!!!!!!!!!!!!!: %s "), *NewController->GetName());
	////UE_LOG(LogTemp, Log, TEXT("========================="));

	// if(Cast<APlayerController>(NewController))
	// {
	// 	////UE_LOG(LogTemp, Log, TEXT("Mouse 커서 없앴음 777"));
	// 	Cast<APlayerController>(NewController)->bShowMouseCursor = false;
	// 	
	// }

	//bIsPossessed = true;
	//NewController->bShowMouseCursor = false;
}

void APlayerCharacter::OnRep_Owner()
{
	FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	////UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);
	////UE_LOG(LogTemp, Log, TEXT("========================="));
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		////UE_LOG(LogTemp, Log, TEXT("OnRep_Owner!!!!!!!!!!!!!!!!:  %s"), *GetOwner()->GetActorNameOrLabel());
	}
	else
	{
		////UE_LOG(LogTemp, Log, TEXT("No Owner "));
	}
	Super::OnRep_Owner();





	OwnerActor = GetOwner();
	if (OwnerActor)
	{
		
		////UE_LOG(LogTemp, Log, TEXT("OnRep_Owner!!!!!!!!!!!!!!!!:  %s"), *GetOwner()->GetActorNameOrLabel());
	}
	else
	{
		////UE_LOG(LogTemp, Log, TEXT("No Owner "));
	}
	////UE_LOG(LogTemp, Log, TEXT("========================="));
}

void APlayerCharacter::RPC_Item_Owner_Success_Implementation(APickUpItem* InteractItem, APawn* TargetPawn)
{
	if (TargetPawn->IsLocallyControlled()) {
		//UE_LOG(LogTemp, Log, TEXT("대박 여기까지 왔다22222?"));
		InteractItem->Interact(this);
	}
	if (TargetPawn->HasAuthority()) {
		//UE_LOG(LogTemp, Log, TEXT("대박 여기까지 왔다?"));
	}
	//UE_LOG(LogTemp, Log, TEXT("interact 4"));
	//UE_LOG(LogTemp, Log, TEXT("Owner RPC  SUCCESS 성공 !!! %s %s"), *InteractItem->GetName(), *TargetPawn->GetName());
}

void APlayerCharacter::RPC_Item_Owner_Implementation(APickUpItem* InteractItem, APawn* TargetPawn)
{
	//UE_LOG(LogTemp, Log, TEXT("interact 4"));
	//UE_LOG(LogTemp, Log, TEXT("Owner RPC 성공 !!! %s %s"), *InteractItem->GetName(), *TargetPawn->GetName());
	InteractItem->SetOwner(TargetPawn);
	RPC_Item_Owner_Success(InteractItem, TargetPawn);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = Cast<AInventoryHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->InventoryPanel->InitializePanel(this);
	HUD->NPCConversation->SetCharacter(this);
	
	UNetwork_Manager_R* Network_Manager =  Cast<UNetwork_Manager_R>(GetGameInstance());
	//const ItemManager& ItemManagerInstance = ItemManager::Get();

	ItemManagerInstance = Network_Manager->GetItemManager();
	if (IsLocallyControlled()) {
		IsQuestAccept = Network_Manager->IsQuestAccept;
	}
	
	
	if (!ItemManagerInstance->ItemDataTable)
	{
		//UE_LOG(LogTemp,Warning,TEXT("DB 널"))
	}
	else
	{
		TArray<TSharedPtr<FJsonValue>> ValuesArray = Network_Manager->ValuesArray;
		for (const TSharedPtr<FJsonValue>& Value : ValuesArray)
		{
			// 각 항목을 mapValue 객체로 추출합니다.
			const TSharedPtr<FJsonObject> MapValueObject = Value->AsObject()->GetObjectField(TEXT("mapValue"));
			const TSharedPtr<FJsonObject> TheObject = MapValueObject->GetObjectField(TEXT("fields"));
    
			// 이제 key와 quantity를 fields 객체에서 추출할 수 있습니다.
			const FString ItemKey = TheObject->GetObjectField(TEXT("key"))->GetStringField(TEXT("stringValue"));
			const int32 ItemQuantity = TheObject->GetObjectField(TEXT("quantity"))->GetIntegerField(TEXT("integerValue"));
			
			UItemBase* MakeItem = ItemManagerInstance->MakeItemBaseByKey(this,ItemKey,ItemQuantity);
			PlayerInventory->HandleAddItem(MakeItem);
		
		}

		PlayerInventory->AddMoney(Network_Manager->MoneyFromServer);
	}
	// 현재 실행 환경이 서버인지 클라이언트인지 확인
	// FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	// ////UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);
	if (IsLocallyControlled())
	{
		////UE_LOG(LogTemp, Display, TEXT("Locally controlled: %s"), *GetActorNameOrLabel());
	}
	else if (GetLocalRole() == ROLE_Authority)
	{
		////UE_LOG(LogTemp, Display, TEXT("Server controlled: %s"), *GetActorNameOrLabel());
	}
	else if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		////UE_LOG(LogTemp, Display, TEXT("Simulated proxy: %s"), *GetActorNameOrLabel());
	}
	else if (GetLocalRole() == ROLE_None)
	{
		////UE_LOG(LogTemp, Display, TEXT("No network role: %s"), *GetActorNameOrLabel());
	}

	////UE_LOG(LogTemp, Log, TEXT("========================= %s %s"), *GetName(), *GetActorNameOrLabel());

	if (GetController() != nullptr)
	{
		EnableInput(Cast<ALoginController>(GetController()));
		////UE_LOG(LogTemp, Log, TEXT("Controller is assigned: %s %s"), *GetController()->GetName(), *GetName());
	}
	else
	{
		////UE_LOG(LogTemp, Warning, TEXT("Controller is not assigned."));
	}




	////UE_LOG(LogTemp, Log, TEXT("========================="));

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
		////UE_LOG(LogTemp, Log, TEXT("칼 생성 했자나"));
		MySword = GetWorld()->SpawnActor<ASword>(SwordClass);
		MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket_r"));
		MySword->SetOwner(this);
	}

	if (GunClass)
	{
		////UE_LOG(LogTemp, Log, TEXT("총 생성 했자나"));
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 인벤토리 영역 인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역 ////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	CheckInteraction();


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 인벤토리 영역 인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역 ////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////





	/*if (GetMovementComponent()->IsFalling()) {
		////UE_LOG(LogTemp, Warning, TEXT("IsFalling : True"));
	}
	else {
		////UE_LOG(LogTemp, Warning, TEXT("IsFalling : False"));
	}*/


	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FVector End = Location + Rotation.Vector() * ShootAttackRange;

	if (GetController())
	{
		Cast<ALoginController>(GetController())->Current_Location = Location;
		Cast<ALoginController>(GetController())->Current_Rotatation = Rotation;
	}

	/*DrawDebugLine(GetWorld(), Location, End, FColor::Cyan, false);
	DrawDebugBox(GetWorld(), End, FVector(10, ShootAttackWidth, ShootAttackHeight), Rotation.Quaternion(), FColor::Cyan, false);
	DrawDebugBox(GetWorld(), Location, FVector(10, ShootAttackWidth, ShootAttackHeight), Rotation.Quaternion(), FColor::Cyan, false);*/


	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority) {
		int Temp = ceil(GetWorldTimerManager().GetTimerRemaining(FiringTimer));
		if (CurrentCountdown_Rep != Temp) {
			CurrentCountdown_Rep = Temp;
			CurrentCountdown_Temp = (float)CurrentCountdown_Rep;
		}
	}

	//클라이언트 전용 함수 기능
	if (IsLocallyControlled())
	{

	}

	CurrentCountdown_Temp -= DeltaTime;
	CurrentCountdown = FMath::Clamp(CurrentCountdown_Temp, 0.f, CoolTime);

	/*if (bIsAttacking) {
		////UE_LOG(LogTemp, Display, TEXT("bIsAttacking TRUE"));
	}
	else {
		////UE_LOG(LogTemp, Display, TEXT("bIsAttacking FALSE"));
	}*/

	//SpawnDebugCapsule(Location, FVector(UpperSlashAttackRadius, UpperSlashAttackRadius, UpperSlashAttackHeight));
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	////UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent ..."));
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	////UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent22 ..."));

	//뉴인풋
	//UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//if (Input != nullptr)
	//{
	//	////UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent33 ..."));
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
	//	////UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent44 ..."));
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 인벤토리 영역 인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역 ////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::EndInteract);

	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &APlayerCharacter::OpenInventory);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 인벤토리 영역 인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역인벤토리 영역 ////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void APlayerCharacter::Move(const FInputActionInstance& Instance)
{
	//////UE_LOG(LogTemp,Log,TEXT("Move ..."));
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

		//////UE_LOG(LogTemp, Warning, TEXT("XXXXX : %f, YYYYY : %f"), MovementVector.X, MovementVector.Y);
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

		//////UE_LOG(LogTemp, Warning, TEXT("XXXXX : %f, YYYYY : %f"), LookVector.X, LookVector.Y);
	}
}

void APlayerCharacter::UpperSlash_Implementation()
{
	if (bIsAttacking || bIsUpperSlashCooldown || GetCharacterMovement()->IsFalling()) return;

	bIsUpperSlashCooldown = true;
	bIsUpperSlash = true;
	bIsAttacking = true;

	////UE_LOG(LogTemp, Log, TEXT("Upper Slash init"));
	//FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(FiringTimer, this, &APlayerCharacter::AttackCoolTime, CoolTime, false);

	//GetWorldTimerManager().SetTimer(Handle, this, &APlayerCharacter::StopUpperSlash, UpperSlash_Rate, false);

}

void APlayerCharacter::StopUpperSlash_Implementation()
{
	bIsUpperSlash = false;
	bIsAttacking = false;
}

void APlayerCharacter::OnRep_IsUpperSlash()
{

}

void APlayerCharacter::HandleUpperSlash()
{
	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority) {
		////UE_LOG(LogTemp, Display, TEXT("UpperSlashAttack!!!!!!!!!"));
		UpperSlashAttack();
	}
}


// void APlayerCharacter::OnRep_Possessed()
// {
// 	AController* MyController = GetController();
// 	if(Cast<APlayerController>(MyController))
// 	{
// 		////UE_LOG(LogTemp, Log, TEXT("Mouse 커서 없앴음 777"));
// 		Cast<APlayerController>(MyController)->bShowMouseCursor = false;
// 		
// 	}
// }

void APlayerCharacter::RPC_Drop_Item_Implementation(int key, const int32 RemovedQuantity)
{


	const FVector SpawnLocation{ GetActorLocation() + GetActorForwardVector() * 50.0f };
	const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

	const AItemManager* ItemManager = Cast<UNetwork_Manager_R>(GetGameInstance())->GetItemManager();

	UItemBase* ItemToDrop = ItemManager->MakeItemBaseByKey(this, key, RemovedQuantity);

	//	ItemManager& ItemManagerInstance = ItemManager::Get();
	if (!ItemToDrop) {
		//UE_LOG(LogTemp, Display, TEXT("itemtodrop null... "));
		return;
	}

	ItemManagerInstance->SpawnItem(this, ItemToDrop, SpawnTransform, RemovedQuantity);
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
	DOREPLIFETIME(APlayerCharacter, FiringTimer);
	DOREPLIFETIME(APlayerCharacter, bIsUpperSlashCooldown);
	DOREPLIFETIME(APlayerCharacter, CurrentCountdown_Rep);
	//DOREPLIFETIME(APlayerCharacter, bIsPossessed);

}



void APlayerCharacter::SetCurrentHealth(float healthValue)
{
	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);

		////화면 출력
		//FString HealthMessage = FString::Printf(TEXT("CurrentHealth : %f"), CurrentHealth);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, HealthMessage);

		OnHealthUpdate();
	}
}

void APlayerCharacter::SetMaxHealth(float healthValue)
{
	MaxHealth = healthValue;
	if (GetCurrentHealth() > MaxHealth) SetCurrentHealth(healthValue);
}


void APlayerCharacter::HitEffectEvent_RPC_Implementation()
{
	HitEffectEvent();
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

	if (bIsAttacking || GetCharacterMovement()->IsFalling()) return;
	////UE_LOG(LogTemp, Log, TEXT("START RFIRE"));

	UseControllerRotationYaw_Toggle_Multi(true);

	//////UE_LOG(LogTemp, Display, TEXT("??????????????"));
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
	////UE_LOG(LogTemp, Display, TEXT("FALSE"));

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
	bIsUpperSlashCooldown = false;
}

float APlayerCharacter::GetCountDown()
{
	return CurrentCountdown;
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
		////UE_LOG(LogTemp, Display, TEXT("Attack!!!!!!!!!"));
		ShootAttack();
	}

}

void APlayerCharacter::OnRep_CurrentCountdown_Rep() //OnRep_ : client에서만 실행됨
{
	CurrentCountdown_Temp = (float)CurrentCountdown_Rep;

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
		UNetwork_Manager_R* Network_Manager = Cast<UNetwork_Manager_R>(GetGameInstance());
		if (bIsDead)
		{
			Network_Manager->Respawn_Widget->SetVisibility(ESlateVisibility::Visible);
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(Network_Manager->Respawn_Widget->TakeWidget());
			GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
		else {
			Network_Manager->Respawn_Widget->SetVisibility(ESlateVisibility::Collapsed);
			FInputModeGameOnly InputMode;
			//FInputModeDataBase
			GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		}
	}

	//서버 전용 함수 기능
	if (GetLocalRole() == ROLE_Authority)
	{
		if (bIsDead)
		{
			
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
		// 죽음 상태에서는 무브먼트 끔
		GetCharacterMovement()->DisableMovement();
		//GetCapsuleComponent()->SetHiddenInGame(true);
		//SetActorTickEnabled(false);
		//UE_LOG(LogTemp, Display, TEXT("플레이어 죽음"));
	}
	else {
		//콜리전 켬
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		// 죽음 상태에서는 무브먼트 켬
		GetCharacterMovement()->SetDefaultMovementMode();
		//GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		//UE_LOG(LogTemp, Display, TEXT("플레이어 부활"));
	}
	
}

void APlayerCharacter::Respawn_Implementation()
{
	SetIsDead(false);
	SetCurrentHealth(MaxHealth);
	SetActorLocation(FVector(-4236.f, -11883.f, 200.f));
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

	HitEffectEvent_RPC();

	////화면 출력
	//FString TakeDamageMessage = FString::Printf(TEXT("TakeDamage Damage : %f"), DamageTaken);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TakeDamageMessage);
	//////UE_LOG(LogTemp, Warning, TEXT("TakeDamage Damage : %f"), DamageTaken);

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
			//SpawnDebugSphere(Hit.ImpactPoint, 30);

			FPointDamageEvent DamageEvent(ShootAttackDamage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(ShootAttackDamage, DamageEvent, OwnerController, this);



			////화면 출력
			//FString AttackMessage = FString::Printf(TEXT("Attack Damage : %f"), ShootAttackDamage);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, AttackMessage);
			//////UE_LOG(LogTemp, Warning, TEXT("Attack Damage : %f"), ShootAttackDamage);

			//FString HitActorMessage = FString::Printf(TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, HitActorMessage);
			//////UE_LOG(LogTemp, Warning, TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
		}

	}

}

void APlayerCharacter::ShootAttack()
{

	//if (!HasAuthority()) return;
	//if(!HasLocalNetOwner()) return;
	/*
	FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	////UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);

	if(GetOwner())
	{
		////UE_LOG(LogTemp,Log, TEXT(" Shoot 의 오우너 :  %s "), *GetOwner()->GetName())	;
	}

	if(GetNetOwner()){

		////UE_LOG(LogTemp,Log, TEXT(" Shoot 의 넷 오우너 :  %s "), *GetNetOwner()->GetName())	;

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
			//SpawnDebugSphere(Hit.ImpactPoint, 30);

			//피격 이펙트 생성
			//UGameplayStatics::SpawnEmitterAtLocation(this, ShootHitPaticles, Hit.ImpactPoint, FRotator::ZeroRotator, FVector(ShootHitEffectScale));
			SpawnEmitterAtLocation_Multi(this, ShootHitPaticles, Hit.ImpactPoint, FRotator::ZeroRotator, FVector(ShootHitEffectScale));

			FPointDamageEvent DamageEvent(ShootAttackDamage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(ShootAttackDamage, DamageEvent, GetController(), this);

			////화면 출력
			//FString AttackMessage = FString::Printf(TEXT("Attack Damage : %f"), ShootAttackDamage);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, AttackMessage);
			//////UE_LOG(LogTemp, Warning, TEXT("Attack Damage : %f"), ShootAttackDamage);

			//FString HitActorMessage = FString::Printf(TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, HitActorMessage);
			//////UE_LOG(LogTemp, Warning, TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
		}

		/*DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 3, 0, 5);
		DrawDebugBox(GetWorld(), End, FVector(10, ShootAttackWidth, ShootAttackHeight), Rotation.Quaternion(), FColor::Red, false, 3, 0, 5);*/

	}

}

void APlayerCharacter::UpperSlashAttack()
{

	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	//콜리전 크기
	FVector CollisionExtent = GetSimpleCollisionCylinderExtent();
	//////UE_LOG(LogTemp, Warning, TEXT("CollisionExtent : %f, %f, %f"), CollisionExtent.X, CollisionExtent.Y, CollisionExtent.Z);

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

				////화면 출력
				//FString AttackMessage = FString::Printf(TEXT("Attack Damage : %f"), ShootAttackDamage);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, AttackMessage);
				//////UE_LOG(LogTemp, Warning, TEXT("Attack Damage : %f"), ShootAttackDamage);

				//FString HitActorMessage = FString::Printf(TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, HitActorMessage);
				//////UE_LOG(LogTemp, Warning, TEXT("HitActor : %s"), *HitActor->GetActorNameOrLabel());
			}
		}


		//DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 3, 0, 5);
		//DrawDebugBox(GetWorld(), End, FVector(10, ShootAttackWidth, ShootAttackHeight), Rotation.Quaternion(), FColor::Red, false, 3, 0, 5);
	}

	//SpawnDebugCapsule(AttackLocation, CapsuleSize);

}

void APlayerCharacter::SpawnEmitterAtLocation_Multi_Implementation(const UObject* WorldContextObject, UParticleSystem* Particle, FVector Location, FRotator Rotation, FVector Scale, float DurationSec)
{
	////UE_LOG(LogTemp, Warning, TEXT("SpawnEmitterAtLocation"));
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
	////UE_LOG(LogTemp, Warning, TEXT("DrawDebugCapsule"));
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
