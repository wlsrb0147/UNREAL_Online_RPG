// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyProjectile.h"
#include "EnemyDog.generated.h"

UCLASS()
class ONLINE_RPG_API AEnemyDog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyDog();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool RangeCheck();

private:
	UPROPERTY(VisibleAnywhere)
	FVector SpawnLocation;
	UPROPERTY(VisibleAnywhere)
	FRotator SpawnRotation;
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
	class UParticleSystem *Attack1Particle;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> AttackProjectile;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyDog> EnemySelf;
	void SpawnSelf();
	FTimerHandle TimerHandle_MyFunction;

	UPROPERTY(EditAnywhere)
	float AttackRange = 500;
	
	void SpawnDebugSphere(FVector Location, float Radius);
	void SpawnProjectile();
	void Dead();

};
