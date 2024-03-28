// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDelegates.h"
#include "ItemStruct.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


class UNetwork_Manager_R;
class AItemManager;
struct FItemData;
class UItemBase;
class IItemInteractionInterface;
class AInventoryHUD;
class UInventoryComponent;

USTRUCT()
struct FInteractingData
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* CurrentInteracting;

	FInteractingData() :CurrentInteracting(nullptr) {}

};




UCLASS()
class ONLINE_RPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
public: // ����Ʈ
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected: // ����Ʈ
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// �κ��丮 ���� �κ��丮 �����κ��丮 �����κ��丮 �����κ��丮 �����κ��丮 �����κ��丮 �����κ��丮 �����κ��丮 ���� ////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; }

	UPROPERTY()
	AInventoryHUD* HUD;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IItemInteractionInterface> InteractionTarget;

	float InteractionDistance = 300.0f;

	int8 IsQuestAccept = 0;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	UPROPERTY(EditAnywhere, Category = "Character | Database")
	UDataTable* ItemDataTable;

	FInteractingData InteractionData;

	UPROPERTY()
	AItemManager* ItemManagerInstance;
	void FoundNoInteract();
	void FoundInteract(AActor* NewInteract);
	void BeginInteract();
	void Interact();
	void EndInteract();
	void OpenInventory();
	void UpdateInteractionWidget() const;
	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);
	void CheckInteraction();

	UPROPERTY()
	UNetwork_Manager_R* Network_Manager;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// �κ��丮 ���� �κ��丮 �����κ��丮 �����κ��丮 �����κ��丮 �����κ��丮 �����κ��丮 �����κ��丮 �����κ��丮 ���� ////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////







public:


	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Owner() override;

	UFUNCTION(Server, Reliable)
	void RPC_Item_Owner(APickUpItem* InteractItem, APawn* TargetPawn);

	UFUNCTION(Client, Reliable)
	void RPC_Item_Owner_Success(APickUpItem* InteractItem, APawn* TargetPawn);

	UFUNCTION(Server, Reliable)
	void RPC_Drop_Item(int key, const int32 QuantityToDrop);

	UFUNCTION(Server, Reliable)
	void RPC_Set_Current_Health(float _Val);
	// UPROPERTY(ReplicatedUsing=OnRep_Possessed)
	// bool bIsPossessed = false;
	// UFUNCTION()
	// void OnRep_Possessed();

	/** ??????? ???��?????? */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** ??? ??? ????*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** ???? ??? ????*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** ???? ??? ????. ???? 0?? MaxHealth ????? ??????????? OnHealthUpdate?? ???????. ?????????? ?????? ????.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealth(float healthValue);

	/** ??????????? ????. APawn???? ?????????????*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	//???? ???? ????
	UFUNCTION(BlueprintPure, Category = "State")
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	//???? ???? ????
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsDead(bool IsDead);


	//Shoot ???? ????
	UFUNCTION(BlueprintPure, Category = "State")
	FORCEINLINE bool GetIsShoot() const { return bIsShoot; }
	//Shoot ???? ????
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsShoot(bool IsShoot);

	//ShootAnim ???? ????
	UFUNCTION(BlueprintPure, Category = "State")
	FORCEINLINE bool GetIsShootAnim() const { return bIsShootAnim; }
	//ShootAnim ???? ????
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsShootAnim(bool IsShootAnim);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SpawnShootEffect();

	UPROPERTY(EditAnywhere, Category = "Particle")
	class UParticleSystem* ShootPaticles;

	UPROPERTY(EditAnywhere, Category = "Particle")
	UParticleSystem* ShootHitPaticles;

	UPROPERTY(EditAnywhere, Category = "Particle")
	UParticleSystem* UpperSlashPaticles;

	UPROPERTY(EditAnywhere, Category = "Particle")
	UParticleSystem* UpperSlashFinishPaticles;

	//Sword ?????
	UPROPERTY(EditAnywhere, Category = "State")
	TSubclassOf<class ASword> SwordClass;
	UPROPERTY(EditAnywhere, Category = "State")
	ASword* MySword;

	//Gun ?????
	UPROPERTY(EditAnywhere, Category = "State")
	TSubclassOf<class AGun> GunClass;
	UPROPERTY(EditAnywhere, Category = "State")
	AGun* MyGun;

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCoolTime() const { return CoolTime; }

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Respawn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom Events")
	void HitEffectEvent();

	UFUNCTION(NetMulticast, Reliable)
	void HitEffectEvent_RPC();

protected:


	/** ?��?????? ??? ???. ????? ???????. ?? ???? ???? ?? ?????? ��?????? ??? ??????.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** ?��?????? ???? ???. 0?? ??? ???? ?????? ???????.*/
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	/** ???? ??��? ?????? ???� ???? RepNotify*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** ?????????? ??��? ????. ?????????? ???? ???????, ????????????? RepNotify?? ??????? ???*/
	void OnHealthUpdate();


	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<class AProjectile_dm> ProjectileClass;

	/** */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float CoolTime = 10.f;


	UPROPERTY(ReplicatedUsing = OnRep_IsAttacking)
	bool bIsAttacking;

	//bIsAttacking ???� ???? RepNotify
	UFUNCTION()
	void OnRep_IsAttacking();

	//?????????? bIsAttacking ???��? ????. ?????????? ???? ??? ???, ????????????? RepNotify?? ??????? ???
	void OnIsAttackingUpdate();


	/** ???? ??? ???? ???*/
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Gameplay")
	void StartFire();

	/** ???? ??? ???? ???. ???????��???? StartFire?? ??? ???????? ??????.*/
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void StopFire();

	void AttackCoolTime();

	UFUNCTION(BlueprintCallable)
	float GetCountDown();

	/** ?????? ??????? ???? ???*/
	UFUNCTION(BlueprintCallable)
	void HandleFire();

	UPROPERTY(Replicated)
	FTimerHandle FiringTimer;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentCountdown_Rep)
	int CurrentCountdown_Rep;
	float CurrentCountdown_Temp;
	float CurrentCountdown;

	UFUNCTION()
	void OnRep_CurrentCountdown_Rep();

	/** ???? ?????*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float AttackRate;

	/** true?? ????? */
	bool bIsAttackWeapon;

	/** ????*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartAttack();

	/** ???? ????? ??*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopAttack();

	/** ???? ???????*/
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void HandleAttack();

	/** ???? ?????? ???? ???*/
	FTimerHandle AttackTimer;

	//???? Attack ????
	bool bIsAttack;


	/*UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsFalling;*/

	//???? ???? ????
	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
	bool bIsDead;

	//???? ???? ???� ???? RepNotify
	UFUNCTION()
	void OnRep_IsDead();

	//?????????? ???? ???��? ????. ?????????? ???? ??? ???, ????????????? RepNotify?? ??????? ???
	void OnIsDeadUpdate();

	//???? Shoot ????
	UPROPERTY(ReplicatedUsing = OnRep_IsShoot)
	bool bIsShoot;

	//Shoot ???? ???� ???? RepNotify
	UFUNCTION()
	void OnRep_IsShoot();

	//?????????? Shoot ???��? ????. ?????????? ???? ??? ???, ????????????? RepNotify?? ??????? ???
	void OnIsShootUpdate();

	//???? ShootAnim ????
	UPROPERTY(ReplicatedUsing = OnRep_IsShootAnim)
	bool bIsShootAnim;

	//???? ???? ???� ???? RepNotify
	UFUNCTION()
	void OnRep_IsShootAnim();

	//?????????? ???? ???��? ????. ?????????? ???? ??? ???, ????????????? RepNotify?? ??????? ???
	void OnIsShootAnimUpdate();


	UFUNCTION(BlueprintCallable)
	void StartShootAnim();
	UFUNCTION(BlueprintCallable)
	void EndShootAnim();


	//?????????
	UPROPERTY(ReplicatedUsing = OnRep_IsUpperSlash)
	bool bIsUpperSlash;
	UPROPERTY(Replicated)
	bool bIsUpperSlashCooldown;
	float UpperSlash_Rate = 2.0f;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void StopUpperSlash();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsUpperSlash() const { return bIsUpperSlash; }

	UFUNCTION()
	void OnRep_IsUpperSlash();

	UFUNCTION(BlueprintCallable)
	void HandleUpperSlash();


private:

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* FireDownAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* FireUpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* UpperSlashAction;



protected:
	void Move(const struct FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	UFUNCTION(Server, Reliable)
	void UpperSlash();

private:
	void SpawnDebugSphere(FVector Location, float Radius);
	void SpawnDebugCapsule(FVector Location, FVector CapsuleSize, FColor Color = FColor::Green);
	void CMAttack();
	//UFUNCTION(Server, Reliable, BlueprintCallable)
	void ShootAttack();
	void UpperSlashAttack();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void SpawnEmitterAtLocation_Multi(const UObject* WorldContextObject, UParticleSystem* Particle, FVector Location, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.f), float DurationSec = 0.f);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void SpawnEmitterAtLocation_SetTimer_Multi(const UObject* WorldContextObject, UParticleSystem* Particle, float SetTime, FVector Location, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.f));

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void UseControllerRotationYaw_Toggle_Multi(bool Val);

	UPROPERTY(EditAnywhere, Category = "Attack")
	float ShootAttackRange = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float ShootAttackDamage = 5.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float ShootAttackWidth = 50.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float ShootAttackHeight = 50.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float ShootHitEffectScale = 0.1f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float ShootEffectScale = 1.f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float UpperSlashAttackDamage = 20.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float UpperSlashAttackRadius = 10.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float UpperSlashAttackHeight = 10.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float UpperSlashAttackDistance = 10.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float UpperSlashEffectScale = 1.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float UpperSlashEffectFinishScale = 1.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float UpperSlashEffectOffsetZ = 0.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float UpperSlashEffectDurationSec = 1.f;

};
