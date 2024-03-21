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


	AItemManager* ItemManagerInstance;

protected:
	// Called when the game starts or when spawned

public:
	// Called every frame
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool RangeCheck();

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
	bool IsDead = false;


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
	float AttackRange = 500;

	void SpawnDebugSphere(FVector Location, float Radius);
	void SpawnProjectile();
	void Dead();
	ACMSpawnManager* SpawnManager;

};