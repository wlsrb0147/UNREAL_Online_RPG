// Copyright Epic Games, Inc. All Rights Reserved.

#include "Online_RPGGameMode.h"
#include "Online_RPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOnline_RPGGameMode::AOnline_RPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
