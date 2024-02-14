// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WIdget_Login.generated.h"

/**
 * 
 */
UCLASS()
class ONLINE_RPG_API UWIdget_Login : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void JoinClick();


	UFUNCTION(BlueprintCallable)
	void LoginClick(FString id, FString pw);

	UFUNCTION(BlueprintCallable)
	void ResistClick(FString id, FString pw, FString nickname);
	
	
};
