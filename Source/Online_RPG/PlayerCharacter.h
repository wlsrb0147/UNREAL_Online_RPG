// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ONLINE_RPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Owner() override;

	/** ������Ƽ ���ø����̼� */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** �ִ� ü�� ����*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** ���� ü�� ����*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** ���� ü�� ����. ���� 0�� MaxHealth ���̷� ���������ϰ� OnHealthUpdate�� ȣ���մϴ�. ���������� ȣ��Ǿ�� �մϴ�.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	/** �������?�޴� �̺�Ʈ. APawn���� �������̵�˴ϴ�?*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	//���� ���� ����
	UFUNCTION(BlueprintPure, Category = "State")
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	//���� ���� ����
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsDead(bool IsDead);


	//Shoot ���� ����
	UFUNCTION(BlueprintPure, Category = "State")
	FORCEINLINE bool GetIsShoot() const { return bIsShoot; }
	//Shoot ���� ����
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsShoot(bool IsShoot);

	//ShootAnim ���� ����
	UFUNCTION(BlueprintPure, Category = "State")
	FORCEINLINE bool GetIsShootAnim() const { return bIsShootAnim; }
	//ShootAnim ���� ����
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

	//Sword Ŭ����
	UPROPERTY(EditAnywhere, Category = "State")
	TSubclassOf<class ASword> SwordClass;
	UPROPERTY(EditAnywhere, Category = "State")
	ASword* MySword;

	//Gun Ŭ����
	UPROPERTY(EditAnywhere, Category = "State")
	TSubclassOf<class AGun> GunClass;
	UPROPERTY(EditAnywhere, Category = "State")
	AGun* MyGun;

	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** �÷��̾��� �ִ� ü��. ü���� �ִ��Դϴ�. �� ���� ���� �� ���۵Ǵ� ĳ������ ü�� ���Դϴ�.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** �÷��̾��� ���� ü��. 0�� �Ǹ� ���� ������ ���ֵ˴ϴ�.*/
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	/** ���� ü�¿� ������ ���濡 ���� RepNotify*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** ������Ʈ�Ǵ� ü�¿� ����. ���������� ���� ���?ȣ��, Ŭ���̾�Ʈ������ RepNotify�� �����Ͽ� ȣ��*/
	void OnHealthUpdate();


	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<class AProjectile_dm> ProjectileClass;

	/** */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float CoolTime;


	UPROPERTY(ReplicatedUsing = OnRep_IsAttacking)
	bool bIsAttacking;

	//bIsAttacking ���濡 ���� RepNotify
	UFUNCTION()
	void OnRep_IsAttacking();

	//������Ʈ�Ǵ� bIsAttacking ���¿� ����. ���������� ���� ��� ȣ��, Ŭ���̾�Ʈ������ RepNotify�� �����Ͽ� ȣ��
	void OnIsAttackingUpdate();


	/** ���� �߻� ���� �Լ�*/
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Gameplay")
	void StartFire();

	/** ���� �߻� ���� �Լ�. ȣ��Ǹ�?�÷��̾ StartFire�� �ٽ� �����?�� �ֽ��ϴ�.*/
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void StopFire();

	void AttackCoolTime();

	/** �߻�ü�� �����ϴ� ���� �Լ�*/
	UFUNCTION(BlueprintCallable)
	void HandleFire();

	/** ���� ���̿� �߻� �ӵ� ����̸�?�ִ� Ÿ�̸� �ڵ�*/
	FTimerHandle FiringTimer;


	/** ���� ��Ÿ��*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float AttackRate;

	/** true�� ��Ÿ�� */
	bool bIsAttackWeapon;

	/** ����*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartAttack();

	/** ���� ��Ÿ�� ��*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopAttack();

	/** ���� �����Լ�*/
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void HandleAttack();

	/** ���� ��Ÿ�ӿ� Ÿ�̸� �ڵ�*/
	FTimerHandle AttackTimer;

	//���� Attack ����
	bool bIsAttack;


	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsFalling;

	//���� ���� ����
	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
	bool bIsDead;

	//���� ���� ���濡 ���� RepNotify
	UFUNCTION()
	void OnRep_IsDead();

	//������Ʈ�Ǵ� ���� ���¿� ����. ���������� ���� ��� ȣ��, Ŭ���̾�Ʈ������ RepNotify�� �����Ͽ� ȣ��
	void OnIsDeadUpdate();

	//���� Shoot ����
	UPROPERTY(ReplicatedUsing = OnRep_IsShoot)
	bool bIsShoot;

	//Shoot ���� ���濡 ���� RepNotify
	UFUNCTION()
	void OnRep_IsShoot();

	//������Ʈ�Ǵ� Shoot ���¿� ����. ���������� ���� ��� ȣ��, Ŭ���̾�Ʈ������ RepNotify�� �����Ͽ� ȣ��
	void OnIsShootUpdate();

	//���� ShootAnim ����
	UPROPERTY(ReplicatedUsing = OnRep_IsShootAnim)
	bool bIsShootAnim;

	//���� ���� ���濡 ���� RepNotify
	UFUNCTION()
	void OnRep_IsShootAnim();

	//������Ʈ�Ǵ� ���� ���¿� ����. ���������� ���� ��� ȣ��, Ŭ���̾�Ʈ������ RepNotify�� �����Ͽ� ȣ��
	void OnIsShootAnimUpdate();


	UFUNCTION(BlueprintCallable)
	void StartShootAnim();
	UFUNCTION(BlueprintCallable)
	void EndShootAnim();


	//���۽�����
	UPROPERTY(ReplicatedUsing = OnRep_IsUpperSlash)
	bool bIsUpperSlash;
	float UpperSlash_Rate = 2.0f;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void StopUpperSlash();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsUpperSlash() const;

	UFUNCTION()
	void OnRep_IsUpperSlash();

	UFUNCTION(BlueprintCallable)
	void HandleUpperSlash();




public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	void SpawnEmitterAtLocation_Multi(const UObject* WorldContextObject, UParticleSystem* Particle, FVector Location, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.f), UParticleSystemComponent* ParticleSystemComponent = nullptr);

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
	float UpperSlashEffectOffsetZ = 0.f;

};
