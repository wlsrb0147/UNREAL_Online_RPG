// Fill out your copyright notice in the Description page of Project Settings.


#include "TiTle_GameMode.h"
#include "Blueprint/UserWidget.h"


void ATiTle_GameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("RED RED"));

	if(!GetWorld()) UE_LOG(LogTemp, Log, TEXT("GET world 가 없는건 말이 안돼잖아"));
	
	if(!Login_Widget_CLASS) UE_LOG(LogTemp, Log, TEXT("flag 1111")); 
	Login_Widget = CreateWidget<UUserWidget>(GetWorld(), Login_Widget_CLASS);
	if(!Login_Widget_CLASS) UE_LOG(LogTemp, Log, TEXT("flag 2222")); 
	if (!Login_Widget) return;
	if(!Login_Widget_CLASS) UE_LOG(LogTemp, Log, TEXT("flag 3333")); 

	
	Login_Widget->AddToViewport();
	UE_LOG(LogTemp, Log, TEXT("RED RED 333333333333"));
}
