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

	/** ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ¼ ï¿½ï¿½ï¿½Ã¸ï¿½ï¿½ï¿½ï¿½Ì¼ï¿½ */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** ï¿½Ö´ï¿½ Ã¼ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** ï¿½ï¿½ï¿½ï¿½ Ã¼ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** ï¿½ï¿½ï¿½ï¿½ Ã¼ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½. ï¿½ï¿½ï¿½ï¿½ 0ï¿½ï¿½ MaxHealth ï¿½ï¿½ï¿½Ì·ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ï°ï¿½ OnHealthUpdateï¿½ï¿½ È£ï¿½ï¿½ï¿½Õ´Ï´ï¿½. ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ È£ï¿½ï¿½Ç¾ï¿½ï¿½ ï¿½Õ´Ï´ï¿½.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	/** ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½Þ´ï¿½ ï¿½Ìºï¿½Æ®. APawnï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ìµï¿½Ë´Ï´ï¿?*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	//Á×À½ »óÅÂ °ÔÅÍ
	UFUNCTION(BlueprintPure, Category = "State")
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	//Á×À½ »óÅÂ ¼¼ÅÍ
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsDead(bool IsDead);

	void SetIsDead();


	//Shoot »óÅÂ °ÔÅÍ
	UFUNCTION(BlueprintPure, Category = "State")
	FORCEINLINE bool GetIsShoot() const { return bIsShoot; }
	//Shoot »óÅÂ ¼¼ÅÍ
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsShoot(bool IsShoot);

	//Sword Å¬·¡½º
	UPROPERTY(EditAnywhere, Category = "State")
	TSubclassOf<class ASword> SwordClass;
	UPROPERTY(EditAnywhere, Category = "State")
	ASword* MySword;
	
	//Gun Å¬·¡½º
	UPROPERTY(EditAnywhere, Category = "State")
	TSubclassOf<class AGun> GunClass;
	UPROPERTY(EditAnywhere, Category = "State")
	AGun* MyGun;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** ï¿½Ã·ï¿½ï¿½Ì¾ï¿½ï¿½ï¿½ ï¿½Ö´ï¿½ Ã¼ï¿½ï¿½. Ã¼ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´ï¿½ï¿½Ô´Ï´ï¿½. ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ÛµÇ´ï¿½ Ä³ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ã¼ï¿½ï¿½ ï¿½ï¿½ï¿½Ô´Ï´ï¿½.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** ï¿½Ã·ï¿½ï¿½Ì¾ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ Ã¼ï¿½ï¿½. 0ï¿½ï¿½ ï¿½Ç¸ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ÖµË´Ï´ï¿½.*/
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	/** ï¿½ï¿½ï¿½ï¿½ Ã¼ï¿½Â¿ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½æ¿¡ ï¿½ï¿½ï¿½ï¿½ RepNotify*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ®ï¿½Ç´ï¿½ Ã¼ï¿½Â¿ï¿½ ï¿½ï¿½ï¿½ï¿½. ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿?È£ï¿½ï¿½, Å¬ï¿½ï¿½ï¿½Ì¾ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ RepNotifyï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï¿ï¿½ È£ï¿½ï¿½*/
	void OnHealthUpdate();


	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<class AProjectile_dm> ProjectileClass;

	/** */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	/** trueï¿½ï¿½ ï¿½ï¿½ï¿?ï¿½ß»ï¿½Ã¼ï¿½ï¿½ ï¿½ß»ï¿½ï¿½Ï´ï¿½ ï¿½ï¿½ï¿½Î¼ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ô´Ï´ï¿½. */
	bool bIsFiringWeapon;

	/** ï¿½ï¿½ï¿½ï¿½ ï¿½ß»ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ô¼ï¿½*/
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Gameplay")
	void StartFire();

	/** ï¿½ï¿½ï¿½ï¿½ ï¿½ß»ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ô¼ï¿½. È£ï¿½ï¿½Ç¸ï¿?ï¿½Ã·ï¿½ï¿½Ì¾î°¡ StartFireï¿½ï¿½ ï¿½Ù½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ ï¿½Ö½ï¿½ï¿½Ï´ï¿½.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	/** ï¿½ß»ï¿½Ã¼ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï´ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ô¼ï¿½*/
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void HandleFire();

	/** ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ì¿ï¿½ ï¿½ß»ï¿½ ï¿½Óµï¿½ ï¿½ï¿½ï¿½ï¿½Ì¸ï¿?ï¿½Ö´ï¿½ Å¸ï¿½Ì¸ï¿½ ï¿½Úµï¿½*/
	FTimerHandle FiringTimer;


	/** °ø°Ý ÄðÅ¸ÀÓ*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float AttackRate;

	/** true¸é ÄðÅ¸ÀÓ */
	bool bIsAttackWeapon;

	/** °ø°Ý*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartAttack();

	/** °ø°Ý ÄðÅ¸ÀÓ ³¡*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopAttack();

	/** °ø°Ý ¼­¹öÇÔ¼ö*/
	UFUNCTION(Server, Reliable)
	void HandleAttack();

	/** °ø°Ý ÄðÅ¸ÀÓ¿ë Å¸ÀÌ¸Ó ÇÚµé*/
	FTimerHandle AttackTimer;

	//ÇöÀç Attack »óÅÂ
	bool bIsAttack;


	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsFalling;

	//ÇöÀç Á×À½ »óÅÂ
	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
	bool bIsDead;

	//Á×À½ »óÅÂ º¯°æ¿¡ ´ëÇÑ RepNotify
	UFUNCTION()
	void OnRep_IsDead();

	//¾÷µ¥ÀÌÆ®µÇ´Â Á×À½ »óÅÂ¿¡ ¹ÝÀÀ. ¼­¹ö¿¡¼­´Â ¼öÁ¤ Áï½Ã È£Ãâ, Å¬¶óÀÌ¾ðÆ®¿¡¼­´Â RepNotify¿¡ ¹ÝÀÀÇÏ¿© È£Ãâ
	void OnIsDeadUpdate();

	//ÇöÀç Shoot »óÅÂ
	UPROPERTY(ReplicatedUsing = OnRep_IsShoot)
	bool bIsShoot;

	//Shoot »óÅÂ º¯°æ¿¡ ´ëÇÑ RepNotify
	UFUNCTION()
	void OnRep_IsShoot();

	//¾÷µ¥ÀÌÆ®µÇ´Â Shoot »óÅÂ¿¡ ¹ÝÀÀ. ¼­¹ö¿¡¼­´Â ¼öÁ¤ Áï½Ã È£Ãâ, Å¬¶óÀÌ¾ðÆ®¿¡¼­´Â RepNotify¿¡ ¹ÝÀÀÇÏ¿© È£Ãâ
	void OnIsShootUpdate();


	//¾îÆÛ½½·¡½Ã
	UPROPERTY(ReplicatedUsing=OnRep_IsUpperSlash)
	bool bIsUpperSlash;
	float UpperSlash_Rate = 2.0f;

	UFUNCTION()
	void StopUpperSlash();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsUpperSlash() const;

	UFUNCTION()
	void OnRep_IsUpperSlash();


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
	UFUNCTION(Server, Reliable)
	void UpperSlash();

private:
	//?„ì‹œ
	void SpawnDebugSphere(FVector Location, float Radius);
	void CMAttack();
	UPROPERTY(EditAnywhere, Category = "Attack")
	float CMAttackRange = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float CMAttackDamage = 5.f;

};
