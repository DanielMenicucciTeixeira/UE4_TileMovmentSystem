// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMovementPlayerController.h"
#include "../TileBaseMovementDemoCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


void ATileMovementPlayerController::DrawPathToCursor()
{
	for (const auto& cell : PathToDraw.CellsInPath) cell->SetColorByState();
	PathToDraw.Empty();
	if (TracePathToCursor(PathToDraw))
	{
		if (MovementColors.Num() <= 0) for (auto& cell : PathToDraw.CellsInPath) cell->Color = FColor::Blue;
		else for (int i = 0; i < PathToDraw.CellsInPath.Num(); i++)
		{
			PathToDraw.CellsInPath[i]->Color = ChooseCellColor(PathToDraw.CellCosts[i]);
		}
	}
}

bool ATileMovementPlayerController::TracePathToCursor(FPath& outPath)
{
	FHitResult TraceHitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
	if (SelectedCharacter)
	{
		FVector start = SelectedCharacter->GetActorLocation();
		FVector end = TraceHitResult.Location;

		return GridManager->FindPathByLocation(outPath, start, end);
	}
	return false;
}

FColor ATileMovementPlayerController::ChooseCellColor(float moveCost)
{
	for (int i = 0; i < MovementColors.Num(); i ++)
	{
		if (moveCost <= MovementColors[i].MoveLimit) return MovementColors[i].Color;
	}
	return OverLimitColor;
}

void ATileMovementPlayerController::SetNewWalkPath()
{
	PathToWalk.Empty();
	PathIndex = 0;
	if (TracePathToCursor(PathToWalk))
	{
		if (MaximumMovement > 0 && PathToWalk.CellCosts.Last() > MaximumMovement)
		{
			Walk = false;
			PathToWalk.Empty();
		}
		else Walk = true;
	}
}

void ATileMovementPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	ATileBaseMovementDemoCharacter* MyPawn = Cast<ATileBaseMovementDemoCharacter>(GetPawn());
	if (MyPawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("%i"), PathIndex));

		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
	}
}

void ATileMovementPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MoveToLocation", IE_Pressed, this, &ATileMovementPlayerController::SetNewWalkPath);

}

void ATileMovementPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ATileMovementPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	if(GetPawn()) SelectedCharacter = GetPawn();
}

void ATileMovementPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(SelectedCharacter) DrawPathToCursor();

	if (PathToWalk.CellsInPath.Num() <= PathIndex) Walk = false;
	else
	{
		float const Distance = FVector::Dist(PathToWalk.CellsInPath[PathIndex]->Location, GetPawn()->GetActorLocation());

		if (Distance <= DistanceToCellTolerance)
		{
			PathIndex++;
			if (PathToWalk.CellsInPath.Num() <= PathIndex) Walk = false;
		}
		if (Walk) SetNewMoveDestination(PathToWalk.CellsInPath[PathIndex]->Location);
	}
}

ATileMovementPlayerController::ATileMovementPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
