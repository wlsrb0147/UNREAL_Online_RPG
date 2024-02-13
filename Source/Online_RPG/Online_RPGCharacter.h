//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
//#include "Logging/LogMacros.h"
//#include "Online_RPGCharacter.generated.h"
//
//class USpringArmComponent;
//class UCameraComponent;
//class UInputMappingContext;
//class UInputAction;
//struct FInputActionValue;
//
//DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
//
//UCLASS(config = Game)
//class AOnline_RPGCharacter : public ACharacter
//{
//	GENERATED_BODY()
//
//	/** Camera boom positioning the camera behind the character */
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
//	USpringArmComponent* CameraBoom;
//
//	/** Follow camera */
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
//	UCameraComponent* FollowCamera;
//
//	/** MappingContext */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
//	UInputMappingContext* DefaultMappingContext;
//
//	/** Jump Input Action */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
//	UInputAction* JumpAction;
//
//	/** Move Input Action */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
//	UInputAction* MoveAction;
//
//	/** Look Input Action */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
//	UInputAction* LookAction;
//
//public:
//	AOnline_RPGCharacter();
//
//	/** 프로퍼티 리플리케이션 */
//	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
//
//	/** 최대 체력 게터*/
//	UFUNCTION(BlueprintPure, Category = "Health")
//	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
//
//	/** 현재 체력 게터*/
//	UFUNCTION(BlueprintPure, Category = "Health")
//	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
//
//	/** 현재 체력 세터. 값을 0과 MaxHealth 사이로 범위제한하고 OnHealthUpdate를 호출합니다. 서버에서만 호출되어야 합니다.*/
//	UFUNCTION(BlueprintCallable, Category = "Health")
//	void SetCurrentHealth(float healthValue);
//
//	/** 대미지를 받는 이벤트. APawn에서 오버라이드됩니다.*/
//	UFUNCTION(BlueprintCallable, Category = "Health")
//	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
//
//	/** ?꾨줈?쇳떚 由ы뵆由ъ??댁뀡 */
//	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
//protected:
//
//	/** Called for movement input */
//	void Move(const FInputActionValue& Value);
//
//	/** Called for looking input */
//	void Look(const FInputActionValue& Value);
//
//
//	/** 플레이어의 최대 체력. 체력의 최댓값입니다. 이 값은 스폰 시 시작되는 캐릭터의 체력 값입니다.*/
//	UPROPERTY(EditDefaultsOnly, Category = "Health")
//	float MaxHealth;
//
//	/** 플레이어의 현재 체력. 0이 되면 죽은 것으로 간주됩니다.*/
//	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
//	float CurrentHealth;
//
//	/** 현재 체력에 가해진 변경에 대한 RepNotify*/
//	UFUNCTION()
//	void OnRep_CurrentHealth();
//
//	/** 업데이트되는 체력에 반응. 서버에서는 수정 즉시 호출, 클라이언트에서는 RepNotify에 반응하여 호출*/
//	void OnHealthUpdate();
//
//
//	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
//	TSubclassOf<class AProjectile_dm> ProjectileClass;
//
//	/** 발사 딜레이, 단위는 초. 테스트 발사체의 발사 속도를 제어하는 데 사용되지만, 서버 함수의 추가분이 SpawnProjectile을 입력에 직접 바인딩하지 않게 하는 역할도 합니다.*/
//	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
//	float FireRate;
//
//	/** true인 경우 발사체를 발사하는 프로세스 도중입니다. */
//	bool bIsFiringWeapon;
//
//	/** 무기 발사 시작 함수*/
//	UFUNCTION(BlueprintCallable, Category = "Gameplay")
//	void StartFire();
//
//	/** 무기 발사 종료 함수. 호출되면 플레이어가 StartFire를 다시 사용할 수 있습니다.*/
//	UFUNCTION(BlueprintCallable, Category = "Gameplay")
//	void StopFire();
//
//	/** 발사체를 스폰하는 서버 함수*/
//	UFUNCTION(Server, Reliable)
//	void HandleFire();
//
//	/** 스폰 사이에 발사 속도 딜레이를 넣는 타이머 핸들*/
//	FTimerHandle FiringTimer;
//
//
//protected:
//	// APawn interface
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//
//	// To add mapping context
//	virtual void BeginPlay();
//
//	/** ?뚮젅?댁뼱??理쒕? 泥대젰. 泥대젰??理쒕뙎媛믪엯?덈떎. ??媛믪? ?ㅽ룿 ???쒖옉?섎뒗 罹먮┃?곗쓽 泥대젰 媛믪엯?덈떎.*/
//	UPROPERTY(EditDefaultsOnly, Category = "Health")
//	float MaxHealth;
//
//	/** ?뚮젅?댁뼱???꾩옱 泥대젰. 0???섎㈃ 二쎌? 寃껋쑝濡?媛꾩＜?⑸땲??*/
//	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
//	float CurrentHealth;
//
//	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
//	float Mana;
//
//	/** ?꾩옱 泥대젰??媛?댁쭊 蹂寃쎌뿉 ???RepNotify*/
//	UFUNCTION()
//	void OnRep_CurrentHealth();
//
//	/** ?낅뜲?댄듃?섎뒗 泥대젰??諛섏쓳. ?쒕쾭?먯꽌???섏젙 利됱떆 ?몄텧, ?대씪?댁뼵?몄뿉?쒕뒗 RepNotify??諛섏쓳?섏뿬 ?몄텧*/
//	UFUNCTION()
//	void OnHealthUpdate();
//
//	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
//	TSubclassOf<class ARED_Projectile> ProjectileClass;
//
//	/** 諛쒖궗 ?쒕젅?? ?⑥쐞??珥? ?뚯뒪??諛쒖궗泥댁쓽 諛쒖궗 ?띾룄瑜??쒖뼱?섎뒗 ???ъ슜?섏?留? ?쒕쾭 ?⑥닔??異붽?遺꾩씠 SpawnProjectile???낅젰??吏곸젒 諛붿씤?⑺븯吏 ?딄쾶 ?섎뒗 ??븷???⑸땲??*/
//	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
//	float FireRate;
//
//	/** true??寃쎌슦 諛쒖궗泥대? 諛쒖궗?섎뒗 ?꾨줈?몄뒪 ?꾩쨷?낅땲?? */
//	bool bIsFiringWeapon;
//
//	/** 臾닿린 諛쒖궗 ?쒖옉 ?⑥닔*/
//	UFUNCTION(BlueprintCallable, Category = "Gameplay")
//	void StartFire();
//
//	/** 臾닿린 諛쒖궗 醫낅즺 ?⑥닔. ?몄텧?섎㈃ ?뚮젅?댁뼱媛 StartFire瑜??ㅼ떆 ?ъ슜?????덉뒿?덈떎.*/
//	UFUNCTION(BlueprintCallable, Category = "Gameplay")
//	void StopFire();
//
//	/** 諛쒖궗泥대? ?ㅽ룿?섎뒗 ?쒕쾭 ?⑥닔*/
//	UFUNCTION(Server, Reliable)
//	void HandleFire();
//
//	/** ?ㅽ룿 ?ъ씠??諛쒖궗 ?띾룄 ?쒕젅?대? ?ｋ뒗 ??대㉧ ?몃뱾*/
//	FTimerHandle FiringTimer;
//
//public:
//	/** Returns CameraBoom subobject **/
//	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
//	/** Returns FollowCamera subobject **/
//	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
//
//	/** 理쒕? 泥대젰 寃뚰꽣*/
//	UFUNCTION(BlueprintPure, Category = "Health")
//	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
//
//	/** ?꾩옱 泥대젰 寃뚰꽣*/
//	UFUNCTION(BlueprintPure, Category = "Health")
//	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
//
//	/** ?꾩옱 泥대젰 ?명꽣. 媛믪쓣 0怨?MaxHealth ?ъ씠濡?踰붿쐞?쒗븳?섍퀬 OnHealthUpdate瑜??몄텧?⑸땲?? ?쒕쾭?먯꽌留??몄텧?섏뼱???⑸땲??*/
//	UFUNCTION(BlueprintCallable, Category = "Health")
//	void SetCurrentHealth(float healthValue);
//
//	/** ?誘몄?瑜?諛쏅뒗 ?대깽?? APawn?먯꽌 ?ㅻ쾭?쇱씠?쒕맗?덈떎.*/
//	UFUNCTION(BlueprintCallable, Category = "Health")
//	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
//};
//
//


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

UCLASS(config = Game)
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

	/** 프로퍼티 리플리케이션 */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	/** 플레이어의 최대 체력. 체력의 최댓값입니다. 이 값은 스폰 시 시작되는 캐릭터의 체력 값입니다.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** 플레이어의 현재 체력. 0이 되면 죽은 것으로 간주됩니다.*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float Mana;

	/** 현재 체력에 가해진 변경에 대한 RepNotify*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** 업데이트되는 체력에 반응. 서버에서는 수정 즉시 호출, 클라이언트에서는 RepNotify에 반응하여 호출*/
	UFUNCTION()
	void OnHealthUpdate();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<class ARED_Projectile> ProjectileClass;

	/** 발사 딜레이, 단위는 초. 테스트 발사체의 발사 속도를 제어하는 데 사용되지만, 서버 함수의 추가분이 SpawnProjectile을 입력에 직접 바인딩하지 않게 하는 역할도 합니다.*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	/** true인 경우 발사체를 발사하는 프로세스 도중입니다. */
	bool bIsFiringWeapon;

	/** 무기 발사 시작 함수*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();

	/** 무기 발사 종료 함수. 호출되면 플레이어가 StartFire를 다시 사용할 수 있습니다.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	/** 발사체를 스폰하는 서버 함수*/
	UFUNCTION(Server, Reliable)
	void HandleFire();

	/** 스폰 사이에 발사 속도 딜레이를 넣는 타이머 핸들*/
	FTimerHandle FiringTimer;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** 최대 체력 게터*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** 현재 체력 게터*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** 현재 체력 세터. 값을 0과 MaxHealth 사이로 범위제한하고 OnHealthUpdate를 호출합니다. 서버에서만 호출되어야 합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	/** 대미지를 받는 이벤트. APawn에서 오버라이드됩니다.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
