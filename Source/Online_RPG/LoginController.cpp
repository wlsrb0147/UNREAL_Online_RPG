// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginController.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ALoginController::ALoginController()
{
	

	bReplicates = true;


}

void ALoginController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoginController, INDEX_OF_PLAYER_CONTROLLER);
	
}

void ALoginController::ChangePawn_Implementation(int PlayerIdx)
{
	UE_LOG(LogTemp, Error, TEXT("콜 스폰 -->  %d 번 %d"), PlayerIdx, UGameplayStatics::GetNumPlayerControllers(this));
	AGameModeBase* MyModeBase = GetWorld()->GetAuthGameMode();

	UWorld* World = GetWorld(); // 현재 GameInstance가 참조하는 월드를 얻습니다.
	if (!World) return;

	// 현재 실행 환경이 서버인지 클라이언트인지 확인
	FString _Role = World->GetNetMode() == NM_DedicatedServer || World->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);

	// 게임 내의 모든 플레이어 컨트롤러의 수를 로깅
	int32 NumPlayerControllers = World->GetNumPlayerControllers();
	UE_LOG(LogTemp, Log, TEXT("게임 내의 플레이어 컨트롤러 수: %d"), NumPlayerControllers);

	// 모든 플레이어 컨트롤러를 순회하며 로깅
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{

			ALoginController* PlayerController = Cast<ALoginController>(PC);

			UE_LOG(LogTemp, Log, TEXT("플레이어 컨트롤러: %s %d %d"), *PC->GetName(), PlayerController->INDEX_OF_PLAYER_CONTROLLER, PlayerIdx);
			
			if (PlayerController != nullptr && PlayerController->INDEX_OF_PLAYER_CONTROLLER == PlayerIdx)
			{
				UE_LOG(LogTemp, Error, TEXT("CallSpawn init"));
				// 새 Pawn 클래스의 인스턴스를 생성합니다.
				SpawnLocation = FVector(-2712.000000,-1030.000000,1187.000000);
				APawn* NewPawn = GetWorld()->SpawnActor<APawn>(SpawnPawn, SpawnLocation, FQuat::Identity.Rotator());
				
				// 새로 생성된 Pawn을 PlayerController에 빙의시킵니다.
				PlayerController->Possess(NewPawn);
				UE_LOG(LogTemp, Log, TEXT("ChangePawn_Implementation 플레이어 컨트롤러: %s"), *NewPawn->GetController()->GetName());
				
				//마우스 커서 숨김
				PlayerController->bShowMouseCursor = false;
			}
		}
	}

}

void ALoginController::BeginPlay()
{
	Super::BeginPlay();

	// if(GetLocalRole() == ROLE_Authority)
	// {
	// 	INDEX_OF_PLAYER_CONTROLLER = NetPlayerIndex;
	// 	UE_LOG(LogTemp, Error, TEXT("INDEX_OF_PLAYER_CONTROLLER 초기화 %d %d" ), NetPlayerIndex, INDEX_OF_PLAYER_CONTROLLER);
	// }
}
