// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Online_RPGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AOnline_RPGCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AOnline_RPGCharacter();

	/** ÇÁ·ÎÆÛÆ¼ ¸®ÇÃ¸®ÄÉÀÌ¼Ç */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** ÃÖ´ë Ã¼·Â °ÔÅÍ*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** ÇöÀç Ã¼·Â °ÔÅÍ*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** ÇöÀç Ã¼·Â ¼¼ÅÍ. °ªÀ» 0°ú MaxHealth »çÀÌ·Î ¹üÀ§Á¦ÇÑÇÏ°í OnHealthUpdate¸¦ È£ÃâÇÕ´Ï´Ù. ¼­¹ö¿¡¼­¸¸ È£ÃâµÇ¾î¾ß ÇÕ´Ï´Ù.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	/** ´ë¹ÌÁö¸¦ ¹Ş´Â ÀÌº¥Æ®. APawn¿¡¼­ ¿À¹ö¶óÀÌµåµË´Ï´Ù.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	/** í”„ë¡œí¼í‹° ë¦¬í”Œë¦¬ì¼€ì´ì…˜ */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


	/** ÇÃ·¹ÀÌ¾îÀÇ ÃÖ´ë Ã¼·Â. Ã¼·ÂÀÇ ÃÖ´ñ°ªÀÔ´Ï´Ù. ÀÌ °ªÀº ½ºÆù ½Ã ½ÃÀÛµÇ´Â Ä³¸¯ÅÍÀÇ Ã¼·Â °ªÀÔ´Ï´Ù.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** ÇÃ·¹ÀÌ¾îÀÇ ÇöÀç Ã¼·Â. 0ÀÌ µÇ¸é Á×Àº °ÍÀ¸·Î °£ÁÖµË´Ï´Ù.*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	/** ÇöÀç Ã¼·Â¿¡ °¡ÇØÁø º¯°æ¿¡ ´ëÇÑ RepNotify*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** ¾÷µ¥ÀÌÆ®µÇ´Â Ã¼·Â¿¡ ¹İÀÀ. ¼­¹ö¿¡¼­´Â ¼öÁ¤ Áï½Ã È£Ãâ, Å¬¶óÀÌ¾ğÆ®¿¡¼­´Â RepNotify¿¡ ¹İÀÀÇÏ¿© È£Ãâ*/
	void OnHealthUpdate();


	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<class AProjectile_dm> ProjectileClass;

	/** ¹ß»ç µô·¹ÀÌ, ´ÜÀ§´Â ÃÊ. Å×½ºÆ® ¹ß»çÃ¼ÀÇ ¹ß»ç ¼Óµµ¸¦ Á¦¾îÇÏ´Â µ¥ »ç¿ëµÇÁö¸¸, ¼­¹ö ÇÔ¼öÀÇ Ãß°¡ºĞÀÌ SpawnProjectileÀ» ÀÔ·Â¿¡ Á÷Á¢ ¹ÙÀÎµùÇÏÁö ¾Ê°Ô ÇÏ´Â ¿ªÇÒµµ ÇÕ´Ï´Ù.*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	/** trueÀÎ °æ¿ì ¹ß»çÃ¼¸¦ ¹ß»çÇÏ´Â ÇÁ·Î¼¼½º µµÁßÀÔ´Ï´Ù. */
	bool bIsFiringWeapon;

	/** ¹«±â ¹ß»ç ½ÃÀÛ ÇÔ¼ö*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();

	/** ¹«±â ¹ß»ç Á¾·á ÇÔ¼ö. È£ÃâµÇ¸é ÇÃ·¹ÀÌ¾î°¡ StartFire¸¦ ´Ù½Ã »ç¿ëÇÒ ¼ö ÀÖ½À´Ï´Ù.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	/** ¹ß»çÃ¼¸¦ ½ºÆùÇÏ´Â ¼­¹ö ÇÔ¼ö*/
	UFUNCTION(Server, Reliable)
	void HandleFire();

	/** ½ºÆù »çÀÌ¿¡ ¹ß»ç ¼Óµµ µô·¹ÀÌ¸¦ ³Ö´Â Å¸ÀÌ¸Ó ÇÚµé*/
	FTimerHandle FiringTimer;
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** í”Œë ˆì´ì–´ì˜ ìµœëŒ€ ì²´ë ¥. ì²´ë ¥ì˜ ìµœëŒ“ê°’ì…ë‹ˆë‹¤. ì´ ê°’ì€ ìŠ¤í° ì‹œ ì‹œì‘ë˜ëŠ” ìºë¦­í„°ì˜ ì²´ë ¥ ê°’ì…ë‹ˆë‹¤.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** í”Œë ˆì´ì–´ì˜ í˜„ì¬ ì²´ë ¥. 0ì´ ë˜ë©´ ì£½ì€ ê²ƒìœ¼ë¡œ ê°„ì£¼ë©ë‹ˆë‹¤.*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float Mana;

	/** í˜„ì¬ ì²´ë ¥ì— ê°€í•´ì§„ ë³€ê²½ì— ëŒ€í•œ RepNotify*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** ì—…ë°ì´íŠ¸ë˜ëŠ” ì²´ë ¥ì— ë°˜ì‘. ì„œë²„ì—ì„œëŠ” ìˆ˜ì • ì¦‰ì‹œ í˜¸ì¶œ, í´ë¼ì´ì–¸íŠ¸ì—ì„œëŠ” RepNotifyì— ë°˜ì‘í•˜ì—¬ í˜¸ì¶œ*/
	UFUNCTION()
	void OnHealthUpdate();

	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Projectile")
	TSubclassOf<class ARED_Projectile> ProjectileClass;

	/** ë°œì‚¬ ë”œë ˆì´, ë‹¨ìœ„ëŠ” ì´ˆ. í…ŒìŠ¤íŠ¸ ë°œì‚¬ì²´ì˜ ë°œì‚¬ ì†ë„ë¥¼ ì œì–´í•˜ëŠ” ë° ì‚¬ìš©ë˜ì§€ë§Œ, ì„œë²„ í•¨ìˆ˜ì˜ ì¶”ê°€ë¶„ì´ SpawnProjectileì„ ì…ë ¥ì— ì§ì ‘ ë°”ì¸ë”©í•˜ì§€ ì•Šê²Œ í•˜ëŠ” ì—­í• ë„ í•©ë‹ˆë‹¤.*/
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float FireRate;

	/** trueì¸ ê²½ìš° ë°œì‚¬ì²´ë¥¼ ë°œì‚¬í•˜ëŠ” í”„ë¡œì„¸ìŠ¤ ë„ì¤‘ì…ë‹ˆë‹¤. */
	bool bIsFiringWeapon;

	/** ë¬´ê¸° ë°œì‚¬ ì‹œì‘ í•¨ìˆ˜*/
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void StartFire();

	/** ë¬´ê¸° ë°œì‚¬ ì¢…ë£Œ í•¨ìˆ˜. í˜¸ì¶œë˜ë©´ í”Œë ˆì´ì–´ê°€ StartFireë¥¼ ë‹¤ì‹œ ì‚¬ìš©í•  ìˆ˜ ìˆìŠµë‹ˆë‹¤.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	/** ë°œì‚¬ì²´ë¥¼ ìŠ¤í°í•˜ëŠ” ì„œë²„ í•¨ìˆ˜*/
	UFUNCTION(Server, Reliable)
	void HandleFire();

	/** ìŠ¤í° ì‚¬ì´ì— ë°œì‚¬ ì†ë„ ë”œë ˆì´ë¥¼ ë„£ëŠ” íƒ€ì´ë¨¸ í•¸ë“¤*/
	FTimerHandle FiringTimer;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** ìµœëŒ€ ì²´ë ¥ ê²Œí„°*/
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** í˜„ì¬ ì²´ë ¥ ê²Œí„°*/
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** í˜„ì¬ ì²´ë ¥ ì„¸í„°. ê°’ì„ 0ê³¼ MaxHealth ì‚¬ì´ë¡œ ë²”ìœ„ì œí•œí•˜ê³  OnHealthUpdateë¥¼ í˜¸ì¶œí•©ë‹ˆë‹¤. ì„œë²„ì—ì„œë§Œ í˜¸ì¶œë˜ì–´ì•¼ í•©ë‹ˆë‹¤.*/
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

	/** ëŒ€ë¯¸ì§€ë¥¼ ë°›ëŠ” ì´ë²¤íŠ¸. APawnì—ì„œ ì˜¤ë²„ë¼ì´ë“œë©ë‹ˆë‹¤.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage( float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser ) override;
};


