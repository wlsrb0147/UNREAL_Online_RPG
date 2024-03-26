// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActor.h"
#include "PickUpItem.h"

class UItemBase;
// Sets default values
ASpawnActor::ASpawnActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASpawnActor::BeginPlay()
{
	Super::BeginPlay();

	InitialBum.ConversationText.Add(FText::FromString(TEXT("이봐, 나좀 도와줘")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("얼마전 괴물들이 바다에서 나와 우리 마을을 습격했어")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("그 괴물 중, 가장 거대한놈이 나의 소중한 피규어를 훔쳐갔어")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("괴물을 쓰러뜨리고, 그 피규어를 되찾아줘")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("아이템은 찾아왔어?")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("말 걸 시간에 빨리 찾아오기나해!!!")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("정말 고마워, 이건 작지만 내 보답이야")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("잘가")));
	InitialBum.ConversationText.Add(FText::FromString(TEXT("이제 너에게 더 볼 일은 없어")));

	ConversationData = InitialBum;
	
	InstanceInteractionData.Quantity = 1;
	InstanceInteractionData.InteractionType = EInteractionType::NPC;
	InstanceInteractionData.Name = FText::FromString(TEXT("거렁뱅이"));

	InteractionData = InstanceInteractionData;
}

void ASpawnActor::BeginInteract()
{
	IItemInteractionInterface::BeginInteract();
}

void ASpawnActor::Interact(APlayerCharacter* PlayerCharacter)
{
	IItemInteractionInterface::Interact(PlayerCharacter);
}
