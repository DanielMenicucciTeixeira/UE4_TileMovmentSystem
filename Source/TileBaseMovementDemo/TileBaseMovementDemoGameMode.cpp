// Copyright Epic Games, Inc. All Rights Reserved.

#include "TileBaseMovementDemoGameMode.h"
#include "TileBaseMovementDemoPlayerController.h"
#include "TileBaseMovementDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATileBaseMovementDemoGameMode::ATileBaseMovementDemoGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATileBaseMovementDemoPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}