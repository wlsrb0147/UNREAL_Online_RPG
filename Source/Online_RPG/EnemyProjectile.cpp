// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    SphreComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Projectile"));
    RootComponent = SphreComponent;
    Mesh->AttachToComponent(SphreComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    UProjectileMovementComponent* ProjectileComponent = this->FindComponentByClass<UProjectileMovementComponent>();
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    
}

void AEnemyProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    // 충돌한 액터가 플레이어인지 확인
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (OtherActor == PlayerController->GetPawn())
    {
        // 플레이어와 충돌한 경우, Hit 정보 가져오기
        FHitResult HitResult;
        FVector StartTrace = GetActorLocation();
        FVector EndTrace = OtherActor->GetActorLocation();
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this); // 충돌 검사에서 이 액터는 무시
        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, CollisionParams);
        if (bHit)
        {
            FVector Q = HitResult.Location;
            SpawnDebugSphere(Q, 200);
            
        }

        // 충돌한 후에 액터 제거
        
    }
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