// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinikillGameMode.h"
#include "MinikillCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMinikillGameMode::AMinikillGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
