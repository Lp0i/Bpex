// Copyright Epic Games, Inc. All Rights Reserved.

#include "BpexGameMode.h"
#include "Bpex/Character/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABpexGameMode::ABpexGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/Blueprint/Character/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
