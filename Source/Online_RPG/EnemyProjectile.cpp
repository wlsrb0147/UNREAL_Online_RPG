// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "PlayerCharacter.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    SphreComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Projectile"));
    RootComponent = SphreComponent;
    Mesh->SetupAttachment(SphreComponent);
    //Mesh->AttachToComponent(SphreComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    UProjectileMovementComponent* ProjectileComponent = this->FindComponentByClass<UProjectileMovementComponent>();

}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnOverlapBegin);
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyProjectile::SpawnDebugSphere(FVector Location, float Radius)
{
    DrawDebugSphere(
        GetWorld(),
        Location,
        Radius,
        20,
        FColor::Red,
        false,
        2, // 스피어를 유지할 시간(초)
        0,
        1
    );
}

void AEnemyProjectile::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    APlayerCharacter* QQ = Cast<APlayerCharacter>(OtherActor);
    if(QQ)
    {
        FDamageEvent DamageEvent; 
        OtherActor->TakeDamage(ProjectileDamage, DamageEvent, GetInstigatorController(), this);
        ////UE_LOG(LogTemp, Warning, TEXT("Actor DamageAble "));
    }
}

