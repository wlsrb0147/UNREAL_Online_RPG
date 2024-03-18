// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginController.h"

#include "Network_Manager_R.h"
#include "PlayerCharacter.h"
#include "Sound_Manager_R.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


ALoginController::ALoginController()
{
	

	//bReplicates = true;
}



void ALoginController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALoginController, INDEX_OF_PLAYER_CONTROLLER);
	
}


void ALoginController::SetLoginID_Implementation(const FString& _Login_ID)
{
	UE_LOG(LogTemp,Log, TEXT("login id rpc"));
	Login_ID = _Login_ID;
}

void ALoginController::PostNetInit()
{

	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		if (NetDriver->ServerConnection)
		{
			UE_LOG(LogTemp, Log, TEXT("Server Connection: %s"), *NetDriver->ServerConnection->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), TEXT("End"));
}

void ALoginController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Log, TEXT(" possess success  %s   in Controller <<<   "), *InPawn->GetActorNameOrLabel());
}

void ALoginController::ChangePawn_Implementation(int PlayerIdx, FVector _SpawnLocation, FRotator _SpawnRotation)
{
	UE_LOG(LogTemp, Error, TEXT("ITS OLDB %s %s "),*_SpawnLocation.ToCompactString(), *_SpawnRotation.ToCompactString());
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
				//UE_LOG(LogTemp, Error, TEXT("CallSpawn init %s "),*PC->GetName());
				// 새 Pawn 클래스의 인스턴스를 생성합니다.
				if(_SpawnLocation.IsNearlyZero()) _SpawnLocation = FVector(-2712.000000,-1030.000000,1187.000000);
				//SpawnLocation = FVector(-2712.000000,-1030.000000,1187.000000);
				APawn* NewPawn = GetWorld()->SpawnActor<APawn>(SpawnPawn, _SpawnLocation, _SpawnRotation);
				
				// 새로 생성된 Pawn을 PlayerController에 빙의시킵니다.
				PlayerController->Possess(NewPawn);
				//NewPawn->SetOwner(PlayerController);

				
				//Cast<APlayerCharacter>(NewPawn)->SetupPlayerInputComponent();


				FInputModeGameOnly InputMode;
				
				PlayerController->SetInputMode(InputMode);
    
				PlayerController->bShowMouseCursor = false;
				
				UE_LOG(LogTemp, Log, TEXT("ChangePawn_Implementation 플레이어 컨트롤러가 가지고 있는것은 ? : %s"), *PlayerController->GetPawn()->GetName());
				
			}
		}
	}

	
	
}

void ALoginController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// if(GetLocalRole() != ROLE_Authority) return;
	// if(GetPawn())
	// UE_LOG(LogTemp, Log, TEXT("Controller TIck..... %s %s"), *GetName(), *GetPawn()->GetName());
	//
}

void ALoginController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	if(HasLocalNetOwner()  && GetGameInstance())
	{
		UNetwork_Manager_R* Instance = Cast<UNetwork_Manager_R>(GetGameInstance());
		Instance->Sound_Play(SOUND_TYPE::BGM_Login, 1, FVector(0,0,0), FRotator(0,0,0), GetPawn());
		// if(Instance && Instance->Get_Sound_Instance())
		// {
		// 	UE_LOG(LogTemp,Log,TEXT("Flag2 %s"), *Instance->Get_Sound_Instance()->GetName());
		// 	ASound_Manager_R* Sound_Manager = Cast<ASound_Manager_R>(Instance->Get_Sound_Instance());
		// 	if(Sound_Manager)
		// 	{
		// 		UE_LOG(LogTemp,Log,TEXT("Flag3"));
		// 		UGameplayStatics::PlaySoundAtLocation(this, Sound_Manager->Login_Sound, FVector(0,0,0), FRotator(0,0,0));
		//
		// 		
		// 	}
		// }
		
	}

	// if(GetLocalRole() == ROLE_Authority)
	// {
	// 	INDEX_OF_PLAYER_CONTROLLER = NetPlayerIndex;
	// 	UE_LOG(LogTemp, Error, TEXT("INDEX_OF_PLAYER_CONTROLLER 초기화 %d %d" ), NetPlayerIndex, INDEX_OF_PLAYER_CONTROLLER);
	// }
}
