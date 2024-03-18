// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound_Manager_R.h"

#include "Network_Manager_R.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASound_Manager_R::ASound_Manager_R()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 오디오 컴포넌트 초기화
	BackgroundMusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundMusicComponent"));
	// 오디오 컴포넌트 초기화
	for (int32 i = 0; i < SoundEffect_NUM; ++i)
	{
		UAudioComponent* NewComponent = CreateDefaultSubobject<UAudioComponent>(FName(*FString::Printf(TEXT("SoundEffectComponent%d"), i)));
		SoundEffectComponents.Add(NewComponent);
		NewComponent->SetupAttachment(RootComponent); // RootComponent가 있다고 가정
	}
	
}

void ASound_Manager_R::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

int32 ASound_Manager_R::Ground_Check(APawn* MyPawn)
{
		//UE_LOG(LogTemp, Log, TEXT("ground checck... %s"), *MyPawn->GetName());
	TArray<AActor*> Overrapping_Actors;
	MyPawn->GetOverlappingActors(Overrapping_Actors);

		//UE_LOG(LogTemp, Log, TEXT("check... %s"), *MyPawn->GetName());
	//겹치는 Actor가 없을 경우
	if (Overrapping_Actors.Num() != 0)
	{
		for(auto AC : Overrapping_Actors)
		{
			//UE_LOG(LogTemp, Log, TEXT("actor : %s"), *AC->GetName());
			if(AC->ActorHasTag("Water")) return 1;
			else if(AC->ActorHasTag("Grass")) return 2;
			else return 0;
		}
	}
	
	FVector CenterPosition = MyPawn->GetActorLocation();
	float Radius = 200.0f;  // 예: 반경 500 유닛
	int32 Count = UGameplayStatics::GrassOverlappingSphereCount(this, Grass_Flower, CenterPosition, Radius);
	if(Count > 0){
		//UE_LOG(LogTemp, Log, TEXT("꽃잔디 찾았다!!!"));
		return 2;
	}

	Count = UGameplayStatics::GrassOverlappingSphereCount(this, Grass_1, CenterPosition, Radius);
	if(Count > 0){
		//UE_LOG(LogTemp, Log, TEXT("잔디1 찾았다!!!"));
		return 2;
	}

	Count = UGameplayStatics::GrassOverlappingSphereCount(this, Grass_2, CenterPosition, Radius);
	if(Count > 0){
		//UE_LOG(LogTemp, Log, TEXT("잔디2 찾았다!!!"));
		return 2;
	}
	
	
	//UE_LOG(LogTemp, Log, TEXT("check...777"));
	return 0;
}

// Called when the game starts or when spawned
void ASound_Manager_R::BeginPlay()
{
	Super::BeginPlay();

	Sound_Map.Add(SOUND_TYPE::BGM_Login,Login_Sound);
	Sound_Map.Add(SOUND_TYPE::BGM_Ingame,Ingame_Sound_queue);
	Sound_Map.Add(SOUND_TYPE::Btn_Click,Btn_Click_Sound);
	Sound_Map.Add(SOUND_TYPE::Btn_GameStart,Btn_Game_Start);
	Sound_Map.Add(SOUND_TYPE::Btn_Click_Fail,Btn_Fail_Sound);
	Sound_Map.Add(SOUND_TYPE::Walk,Walk_Sound_queue);
	Sound_Map.Add(SOUND_TYPE::BGM_Boss,Boss_BGM);
	Sound_Map.Add(SOUND_TYPE::Shoot_Sound_queue,Shoot_Sound_queue);
	Sound_Map.Add(SOUND_TYPE::Fire_Sound, Fire_Sound);
	Sound_Map.Add(SOUND_TYPE::Explosion_Sound,Explosion_Sound);
	Sound_Map.Add(SOUND_TYPE::Walk_Water,Walk_Sound_Water_queue);
	Sound_Map.Add(SOUND_TYPE::Walk_Grass,Walk_Sound_Grass_queue);
	
}

// Called every frame
void ASound_Manager_R::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASound_Manager_R::Sound_Play(SOUND_TYPE Sound_Type, int32 Audio_idx, FVector Location, FRotator Rotator, APawn* MyPawn)
{
	if(Sound_Type == SOUND_TYPE::Walk)
	{
		int GroundType = Ground_Check(MyPawn);
		//UE_LOG(LogTemp, Log, TEXT("Walk 긴해     %d  "), GroundType);
		if(GroundType == 1) Sound_Type = SOUND_TYPE::Walk_Water;
		else if(GroundType == 2) Sound_Type = SOUND_TYPE::Walk_Grass;
	}
	
	USoundBase* NeedSound = Sound_Map.FindRef(Sound_Type);
	//UE_LOG(LogTemp, Log, TEXT("flag0"));
	if (NeedSound == nullptr)  return;
	//USoundBase* NeedSound = *Sound_Map.Find(Sound_Type);
	//if(!NeedSound) return;
	//UE_LOG(LogTemp, Log, TEXT("flag1"));
	UAudioComponent* Sound_For_Audio = nullptr;
	if(Audio_idx==1) Sound_For_Audio = BackgroundMusicComponent;
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("flag2"));
		for(auto AudioCompo : SoundEffectComponents)
		{
			if(!AudioCompo->IsPlaying())
			{
				Sound_For_Audio = AudioCompo;
				break;
			}
			
		}
		if(!Sound_For_Audio) Sound_For_Audio = SoundEffectComponents[0];
	}
	//Sound_For_Audio->SetPaused(true);
	//UE_LOG(LogTemp, Log, TEXT("flag3"));
	Sound_For_Audio->SetSound(NeedSound);
	Sound_For_Audio->SetWorldLocation(Location);
	Sound_For_Audio->Play();
	

	
		
		
}



