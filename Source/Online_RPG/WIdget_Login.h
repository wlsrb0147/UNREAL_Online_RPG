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
	void LoginClick(FString _id, FString _pw);

	UFUNCTION(BlueprintCallable)
	void ResistClick(FString _id, FString _pw, FString _nickname);
	
	UFUNCTION(BlueprintCallable)
	void LoginSuccess();

	UFUNCTION(BlueprintCallable)
	void LoginFail();

	UPROPERTY(EditAnywhere)
	UUserWidget* Current_Widget;

	UFUNCTION(BlueprintCallable)
	void SetCurrentWidget(AActor* Widget);

	UFUNCTION(BlueprintCallable)
	void SetSoonDestroy();

	UFUNCTION(BlueprintCallable)
	void SetDestroy();
};
