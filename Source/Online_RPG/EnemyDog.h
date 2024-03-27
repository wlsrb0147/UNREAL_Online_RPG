#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyProjectile.h"
#include "CMSpawnManager.h"
#include "EnemyDog.generated.h"
class AItemManager;
class AEnemyAIController;
UCLASS()
class ONLINE_RPG_API AEnemyDog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyDog();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Attack();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category = "IS BOSS")
	bool bIsBoss = false;
	
	AItemManager* ItemManagerInstance;

protected:
	// Called when the game starts or when spawned

public:
	// Called every frame
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool RangeCheck(int _AttackNum);

	UFUNCTION(BlueprintPure, Category = "State")
	FORCEINLINE bool GetIsDead() const { return IsDead; }

	UFUNCTION(BlueprintPure, Category = "State")
	FORCEINLINE bool GetIsHit() const { return IsHit; }

	UFUNCTION(BlueprintCallable, Category = "State")
	FORCEINLINE void SetIsDead(bool _tf)  { IsDead = _tf; }

	UFUNCTION(BlueprintCallable, Category = "State")
	FORCEINLINE void SetIsHit(bool _tf)  { IsHit = _tf; }
	
	void SetFreeze(bool _tf);
	void HitOff();
	float GetAttackRange(int _AttackNum);
	
	UPROPERTY(Replicated, VisibleAnywhere)
	bool IsDead = false;
	UPROPERTY(Replicated, VisibleAnywhere)
	bool IsHit = false;
private:
	UPROPERTY(EditAnywhere)
	FVector SpawnLocation;

	UPROPERTY(VisibleAnywhere)
	float Health;
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.f;
	UPROPERTY(EditAnywhere)
	float Damage1 = 1.f;

	

	UPROPERTY(VisibleAnywhere)
	float HitRecoveryTime = 0.5f;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Attack1Particle;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> AttackProjectile;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyDog> EnemySelf;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyAIController> EnemyAICon;
	void SpawnSelf();
	FTimerHandle TimerHandle_MyFunction;

	UPROPERTY(EditAnywhere)
	float AttackRange1 = 600;
	UPROPERTY(EditAnywhere)
	float AttackRange2 = 300;
	void SpawnDebugSphere(FVector Location, float Radius);
	void SpawnProjectile();
	void Dead();
	ACMSpawnManager* SpawnManager;
	UPROPERTY(VisibleAnywhere)
	float AttackTime = 0.f;
	UPROPERTY(EditAnywhere)
	float AttackDelay = 3.f;

	void SetIsCollision(bool IsCollision);

	UPROPERTY(ReplicatedUsing = OnRep_IsCollision)
	bool bIsCollision;

	UFUNCTION()
	void OnRep_IsCollision();

	void OnIsCollisionUpdate();

public:
	UPROPERTY(EditAnywhere, Category = "Particle")
	UParticleSystem* EnemyAttackPaticles;

protected:
	UFUNCTION(BlueprintCallable)
	void HandleAttack();

private:
	UPROPERTY(EditAnywhere, Category = "Attack")
	float EnemyAttackDamage = 20.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float EnemyAttackRadius = 10.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float EnemyAttackHeight = 10.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float EnemyAttackDistance = 10.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float EnemyAttackEffectScale = 1.f;
	/*UPROPERTY(EditAnywhere, Category = "Attack")
	float EnemyEffectFinishScale = 1.f;*/
	UPROPERTY(EditAnywhere, Category = "Attack")
	float EnemyAttackEffectOffsetZ = 0.f;
	/*UPROPERTY(EditAnywhere, Category = "Attack")
	float EnemyEffectDurationSec = 1.f;*/

	void EnemyAttack();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void SpawnEmitterAtLocation_Multi(const UObject* WorldContextObject, UParticleSystem* Particle, FVector Location, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.f));

	void SpawnDebugCapsule(FVector Location, FVector CapsuleSize, FColor Color = FColor::Green);

};