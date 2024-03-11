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
	
	
}

// Called every frame
void ASound_Manager_R::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASound_Manager_R::Sound_Play(SOUND_TYPE Sound_Type, int32 Audio_idx, FVector Location, FRotator Rotator)
{
	USoundBase* NeedSound = *Sound_Map.Find(Sound_Type);
	if(!NeedSound) return;

	UAudioComponent* Sound_For_Audio = nullptr;
	if(Audio_idx==1) Sound_For_Audio = BackgroundMusicComponent;
	else
	{
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
	
	Sound_For_Audio->SetSound(NeedSound);
	Sound_For_Audio->SetWorldLocation(Location);
	Sound_For_Audio->Play();
	

	
		
		
}



