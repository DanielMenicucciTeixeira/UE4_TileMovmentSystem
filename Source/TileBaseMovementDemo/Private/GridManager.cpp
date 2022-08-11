// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridManager.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"
#include "TileBoxCollider.h"
#include "TileModifierVolume.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "TileMovementPlayerController.h"
#include "Algo/Reverse.h"


UCell* AGridManager::GetCellFromCoordinates(int32 x, int32 y, int32 z) const
{
	if (GridCells.Num() == int32(0)) return nullptr;

	if (x >= CellCount.X || y >= CellCount.Y || z >= CellCount.Z)
	{
		return nullptr;
	}

	int index = 0;
	index += x * CellCount.Y * CellCount.Z;
	index += y * CellCount.Z;
	index += z;

	if (index < GridCells.Num())
	{
		return GridCells[index];
		
	}

	return nullptr;
}

bool AGridManager::GetCellIndexFromGridPosition(int32& index, int32 x, int32 y, int32 z) const
{
	if (GridCells.Num() == int32(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("GridCells is empty."));
		return false;
	}
	if (x >= CellCount.X || x < 0 || y >= CellCount.Y || y < 0 || z >= CellCount.Z || z < 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Out of bounds"));
		return false;
	}

	int position = 0;
	position += x * CellCount.Y * CellCount.Z;
	position += y * CellCount.Z;
	position += z;

	if (position < GridCells.Num())
	{
		index = position;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("position was bigger then GridCells size."));
	return false;
}

void AGridManager::CollisionBoxSetUP()
{
	CollisionBox->SetCollisionProfileName("OverlapAll");
	CollisionBox->SetVisibility(false);

	FVector BoxExtent;
	BoxExtent.X = GridSize.X;
	BoxExtent.Y = GridSize.Y;
	BoxExtent.Z = GridSize.Z;

	CollisionBox->SetBoxExtent(BoxExtent / 2);
	CollisionBox->SetRelativeLocation((BoxExtent - FVector(CellSize.X, CellSize.Y, CellSize.Z)) / 2);
}

void AGridManager::SetPlayerControllerReferences()
{
	TArray<AActor*> controllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileMovementPlayerController::StaticClass(), controllers);
	for(const auto& controller : controllers)
	{
		Cast<ATileMovementPlayerController>(controller)->SetGridManager(this);
	}
}

void AGridManager::SetGridSize(float x, float y, float z)
{
	if (x >= 0) GridSize.X = x;
	if (y >= 0) GridSize.Y = y;
	if (z >= 0) GridSize.Z = z;
}

void AGridManager::SetCellCount(int32 x, int32 y, int32 z)
{
	if (x >= 0) CellCount.X = x;
	if (y >= 0) CellCount.Y = y;
	if (z >= 0) CellCount.Z = z;
}

void AGridManager::SetCellSize(float x, float y, float z)
{
	CellSize.X = x;
	CellSize.Y = y;
	CellSize.Z = z;
}

void AGridManager::DrawGridBoxes()
{
	if (GridLines.Num() > 0)	GetWorld()->ForegroundLineBatcher->DrawLines(GridLines);

	for (const auto& cell : GridCells)
	{
		//GetWorld()->ForegroundLineBatcher->DrawPoint(cell->Location, cell->Color, 50, 0.0f);
		if (cell->State != ECellState::DEAD)
		{
			GetWorld()->ForegroundLineBatcher->DrawCircle(cell->Location, FVector(1, 0, 0), FVector(0, 1, 0), cell->Color, 20.0f, 64, 0.0f);
		}
	}
}

void AGridManager::CreateCells()
{
	FVector SpawnLocation;
	FVector StartLocation = GetActorLocation() - CollisionBox->GetScaledBoxExtent();
	StartLocation += FVector(CellSize.X, CellSize.Y, 0.0f) * 0.5f;

	for (int i = 0; i < CellCount.X; i++)
	{
		for (int j = 0; j < CellCount.Y; j++)
		{
			for (int k = 0; k < CellCount.Z; k++)
			{
				SpawnLocation = StartLocation + FVector(CellSize.X * i, CellSize.Y * j, CellSize.Z * k);
				UCell* SpawnedCell = NewObject<UCell>();
				SpawnedCell->Coordinates = FIntVector(i, j, k);
				SpawnedCell->Index = GridCells.Num();
				SpawnedCell->Location = SpawnLocation;
				GridCells.Add(SpawnedCell);
			}
		}
	}
}

void AGridManager::CalculateGridBoxLines()
{
	UCell* start = NewObject<UCell>();
	FVector startLocation;
	UCell* end = NewObject<UCell>();
	FVector endLocation;
	FVector ajustment = FVector(CellSize.X * 0.5f, CellSize.Y * 0.5f, 0.0f);
	uint8 depth = 0;

	if (DrawVerticalLines)
	{
		do
		{
			for (int i = 0; i < CellCount.X; i++)
			{
				for (int j = 0; j < CellCount.Y; j++)
				{
					start = GetCellFromCoordinates(i, j, 0);
					startLocation = start->Location - ajustment;
					end = GetCellFromCoordinates(i, j, CellCount.Z - 1);
					endLocation = end->Location - ajustment + FVector(0, 0, CellSize.Z);
					FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
					GridLines.Add(Line);
				}
				start = GetCellFromCoordinates(i, CellCount.Y - 1, 0);
				startLocation = start->Location - ajustment + FVector(0, CellSize.Y, 0);
				end = GetCellFromCoordinates(i, CellCount.Y - 1, CellCount.Z - 1);
				endLocation = end->Location - ajustment + FVector(0, CellSize.Y, CellSize.Z);
				FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
				GridLines.Add(Line);
			}
			for (int j = 0; j < CellCount.Y; j++)
			{
				start = GetCellFromCoordinates(CellCount.X - 1, j, 0);
				startLocation = start->Location - ajustment + FVector(CellSize.X, 0, 0);
				end = GetCellFromCoordinates(CellCount.X - 1, j, CellCount.Z - 1);
				endLocation = end->Location - ajustment + FVector(CellSize.X, 0, CellSize.Z);
				FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
				GridLines.Add(Line);
			}
			start = GetCellFromCoordinates(CellCount.X - 1, CellCount.Y - 1, 0);
			startLocation = start->Location - ajustment + FVector(CellSize.Y, CellSize.Y, 0);
			end = GetCellFromCoordinates(CellCount.X - 1, CellCount.Y - 1, CellCount.Z - 1);
			endLocation = end->Location - ajustment + FVector(CellSize.Y, CellSize.Y, CellSize.Z);
			FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
			GridLines.Add(Line);
		} while (false);
	}

	if (DrawHorizontalLines)
	{
		do
		{
			for (int i = 0; i < CellCount.X; i++)
			{
				for (int k = 0; k < CellCount.Z; k++)
				{
					start = GetCellFromCoordinates(i, 0, k);
					startLocation = start->Location - ajustment;
					end = GetCellFromCoordinates(i, CellCount.Y - 1, k);
					endLocation = end->Location - ajustment + FVector(0, CellSize.Y, 0);
					FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
					GridLines.Add(Line);
				}
				start = GetCellFromCoordinates(i, 0, CellCount.Z - 1);
				startLocation = start->Location - ajustment + FVector(0, 0, CellSize.Z);
				end = GetCellFromCoordinates(i, CellCount.Y - 1, CellCount.Z - 1);
				endLocation = end->Location - ajustment + FVector(0, CellSize.Y, CellSize.Z);
				FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
				GridLines.Add(Line);
			}
			for (int k = 0; k < CellCount.Z; k++)
			{
				start = GetCellFromCoordinates(CellCount.X - 1, 0, k);
				startLocation = start->Location - ajustment + FVector(CellSize.X, 0, 0);
				end = GetCellFromCoordinates(CellCount.X - 1, CellCount.Y - 1, k);
				endLocation = end->Location - ajustment + FVector(CellSize.X, CellSize.Y, 0);
				FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
				GridLines.Add(Line);
			}
			start = GetCellFromCoordinates(CellCount.X - 1, 0, CellCount.Z - 1);
			startLocation = start->Location - ajustment + FVector(CellSize.X, 0, CellSize.Z);
			end = GetCellFromCoordinates(CellCount.X - 1, CellCount.Y - 1, CellCount.Z - 1);
			endLocation = end->Location - ajustment + FVector(CellSize.X, CellSize.Y, CellSize.Z);
			FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
			GridLines.Add(Line);
		} while (false);

		do
		{
			for (int j = 0; j < CellCount.Y; j++)
			{
				for (int k = 0; k < CellCount.Z; k++)
				{
					start = GetCellFromCoordinates(0, j, k);
					startLocation = start->Location - ajustment;
					end = GetCellFromCoordinates(CellCount.X - 1, j, k);
					endLocation = end->Location - ajustment + FVector(CellSize.X, 0, 0);
					FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
					GridLines.Add(Line);
				}
				start = GetCellFromCoordinates(0, j, CellCount.Z - 1);
				startLocation = start->Location - ajustment + FVector(0, 0, CellSize.Z);
				end = GetCellFromCoordinates(CellCount.X - 1, j, CellCount.Z - 1);
				endLocation = end->Location - ajustment + FVector(CellSize.X, 0, CellSize.Z);
				FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
				GridLines.Add(Line);
			}
			for (int k = 0; k < CellCount.Z; k++)
			{
				start = GetCellFromCoordinates(0, CellCount.Y - 1, k);
				startLocation - start->Location - ajustment + FVector(0, CellSize.Y, 0);
				end = GetCellFromCoordinates(CellCount.X - 1, CellCount.Y - 1, k);
				endLocation = end->Location - ajustment + FVector(CellSize.X, CellSize.Y, 0);
				FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
				GridLines.Add(Line);
			}
			start = GetCellFromCoordinates(0, CellCount.Y - 1, CellCount.Z - 1);
			startLocation = start->Location - ajustment + FVector(0, CellSize.Y, CellSize.Z);
			end = GetCellFromCoordinates(CellCount.X - 1, CellCount.Y - 1, CellCount.Z - 1);
			endLocation = end->Location - ajustment + FVector(CellSize.X, CellSize.Y, CellSize.Z);
			FBatchedLine Line = FBatchedLine(startLocation, endLocation, GridLineColor, 0, GridLineThickness, depth);
			GridLines.Add(Line);
		} while (false);
	}
	
}

void AGridManager::CheckCellBlocks()
{
	for (auto& cell : GridCells)
	{
		if (TileCheckBox)
		{
			FVector location = cell->Location +FVector(0.0f, 0.0f, CellSize.Z * 0.5f);
			TileCheckBox->SetWorldLocation(location);
			TArray<UPrimitiveComponent*> overlappingComponents;
			TileCheckBox->GetOverlappingComponents(overlappingComponents);

			for (auto& component : overlappingComponents)
			{
				if (Cast<UTileBoxCollider>(component))
				{
					Cast<UTileBoxCollider>(component)->AffectedCells.Add(cell);
				}
			}
		}
	}
}

void AGridManager::CalculateSizes()
{
	float gridSizeX = 2 * CollisionBox->GetScaledBoxExtent().X;
	float gridSizeY = 2 * CollisionBox->GetScaledBoxExtent().Y;
	float gridSizeZ = 2 * CollisionBox->GetScaledBoxExtent().Z;

	GridSize = FVector(gridSizeX, gridSizeY, gridSizeZ);
	UE_LOG(LogTemp, Warning,TEXT("%f\t%f\t%f"), gridSizeX, gridSizeY, gridSizeZ);
	
	int32 cellCountX = FGenericPlatformMath::TruncToInt(gridSizeX / CellSize.X);
	if (cellCountX < 1) cellCountX = 1;
	int32 cellCountY = FGenericPlatformMath::TruncToInt(gridSizeY / CellSize.Y);
	if (cellCountY < 1) cellCountY = 1;
	int32 cellCountZ = FGenericPlatformMath::TruncToInt(gridSizeZ / CellSize.Z);
	if (cellCountZ < 1) cellCountZ = 1;

	CellCount = FIntVector(cellCountX, cellCountY, cellCountZ);
}

void AGridManager::SetCell(int32 cellIndex, TEnumAsByte<ECellState> state, float moveCost, int32 modifierPriority)
{
	GridCells[cellIndex]->SetCellParameters(state, moveCost, modifierPriority);
}

UCell* AGridManager::GetClosestCellFromLocation(const FVector& location) const
{
	FVector relativeLocation = location - GetActorLocation();

	float percentX = FMath::Clamp(((relativeLocation.X + GridSize.X / 2) / GridSize.X), 0.0f, 1.0f);
	float percentY = FMath::Clamp(((relativeLocation.Y + GridSize.Y / 2) / GridSize.Y), 0.0f, 1.0f);
	float percentZ = FMath::Clamp(((relativeLocation.Z + GridSize.Z / 2) / GridSize.Z), 0.0f, 1.0f);

	int x = FMath::RoundToInt((CellCount.X - 1) * percentX);
	int y = FMath::RoundToInt((CellCount.Y - 1) * percentY);
	int z = FMath::RoundToInt((CellCount.Z - 1) * percentZ);


	return GetCellFromCoordinates(x, y, z);
}

float AGridManager::GetDistanceBetweenCells(const UCell* cellA, const UCell* cellB, const bool& diagonal, const bool& vertical) const
{
	FIntVector distance = cellA->Coordinates - cellB->Coordinates;
	distance.X = abs(distance.X);
	distance.Y = abs(distance.Y);
	distance.Z = abs(distance.Z);

	if (diagonal)
	{
		int32 bigger, middle, smaller;

		if (distance.X > distance.Y)
		{
			if (distance.X > distance.Z)
			{
				bigger = distance.X;
				if (distance.Y > distance.Z)
				{
					middle = distance.Y;
					smaller = distance.Z;
				}
				else
				{
					middle = distance.Z;
					smaller = distance.Y;
				}
			}
			else
			{
				bigger = distance.Z;
				middle = distance.X;
				smaller = distance.Y;
			}
		}
		else
		{
			if (distance.X < distance.Z)
			{
				smaller = distance.X;
				if (distance.Y > distance.Z)
				{
					bigger = distance.Y;
					middle = distance.Z;
				}
				else
				{
					bigger = distance.Z;
					middle = distance.Y;
				}
			}
			else
			{
				bigger = distance.Y;
				middle = distance.X;
				smaller = distance.Z;
			}
		}

		return (smaller * Diagonal3DCostMultiplier) + ((middle - smaller) * DiagonalCostMultiplier) + (bigger - middle);
	}
	else
	{
		return distance.X + distance.Y + distance.Z;
	}
}

void AGridManager::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//if (Cast<AMiniature>(OtherActor)) Cast<AMiniature>(OtherActor)->InsideGrid = true;
	}
}

void AGridManager::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//if (Cast<AMiniature>(OtherActor)) Cast<AMiniature>(OtherActor)->InsideGrid = false;
	}
}

void AGridManager::InitializeModifierVolumes()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileModifierVolume::StaticClass(), TileModifierVolumes);
	for (auto& volume : TileModifierVolumes)
	{
		Cast<ATileModifierVolume>(volume)->SetGridManager(this);
		Cast<ATileModifierVolume>(volume)->SetActorLocation(GetClosestCellFromLocation(Cast<ATileModifierVolume>(volume)->GetActorLocation())->Location + FVector(0.0f, 0.0f, 0.5f * CellSize.Z));
		Cast<ATileModifierVolume>(volume)->SetComponentsData();
	}
	CheckCellBlocks();
	for (auto& volume : TileModifierVolumes)
	{
		for (auto& collider : Cast<ATileModifierVolume>(volume)->GetComponents())
		{
			if (Cast<UTileBoxCollider>(collider))
			{
				Cast<UTileBoxCollider>(collider)->RefreshAffectedCells();
			}
		}
	}
}

void AGridManager::SetCellNeighbors(UCell* cell)
{
	int32 index = 0;

	for (int i = -1; i <= 1; i += 2)
	{
		if (GetCellIndexFromGridPosition(index, cell->Coordinates.X + i, cell->Coordinates.Y, cell->Coordinates.Z))
		{
			cell->Neighbors.Add(GridCells[index]);
		}
		if (GetCellIndexFromGridPosition(index, cell->Coordinates.X, cell->Coordinates.Y + i, cell->Coordinates.Z))
		{
			cell->Neighbors.Add(GridCells[index]);
		}

		if (CanMoveVertically)
		{
			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X, cell->Coordinates.Y, cell->Coordinates.Z + i))
			{
				cell->Neighbors.Add(GridCells[index]);

			}
		}

		if (CanMoveOnDiagonals)
		{
			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X + i, cell->Coordinates.Y + i, cell->Coordinates.Z))
			{
				cell->DiagonalNeighbors.Add(GridCells[index]);

			}
			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X - i, cell->Coordinates.Y + i, cell->Coordinates.Z))
			{
				cell->DiagonalNeighbors.Add(GridCells[index]);
			}
		}

		if (CanMoveOnDiagonals && CanMoveVertically)
		{
			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X + i, cell->Coordinates.Y, cell->Coordinates.Z + i))
			{
				cell->DiagonalNeighbors.Add(GridCells[index]);
			}
			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X - i, cell->Coordinates.Y, cell->Coordinates.Z + i))
			{
				cell->DiagonalNeighbors.Add(GridCells[index]);
			}
			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X, cell->Coordinates.Y + i, cell->Coordinates.Z + i))
			{
				cell->DiagonalNeighbors.Add(GridCells[index]);
			}
			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X, cell->Coordinates.Y + i, cell->Coordinates.Z - i))
			{
				cell->DiagonalNeighbors.Add(GridCells[index]);
			}

			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X + i, cell->Coordinates.Y + i, cell->Coordinates.Z + i))
			{
				cell->Diagonal3DNeighbors.Add(GridCells[index]);
			}
			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X - i, cell->Coordinates.Y + i, cell->Coordinates.Z + i))
			{
				cell->Diagonal3DNeighbors.Add(GridCells[index]);
			}
			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X + i, cell->Coordinates.Y - i, cell->Coordinates.Z + i))
			{
				cell->Diagonal3DNeighbors.Add(GridCells[index]);
			}
			if (GetCellIndexFromGridPosition(index, cell->Coordinates.X + i, cell->Coordinates.Y + i, cell->Coordinates.Z - i))
			{
				cell->Diagonal3DNeighbors.Add(GridCells[index]);
			}
		}
	}
}

void AGridManager::SetAllCellNeighbors()
{
	for (auto& cell : GridCells)
	{
		SetCellNeighbors(cell);
	}
}

bool AGridManager::FindPathByCell(FPath& outPath, UCell* startCell, UCell* targetCell)
{
	TSet<UCell*> openSet;
	TSet<UCell*> closedSet;
	TSet<UCell*> searchedCells;
	
	openSet.Add(startCell);

	while (openSet.Num() > 0)
	{

		UCell* currentCell = NewObject<UCell>();
		for (const auto& cell : openSet) { currentCell = cell; break; }

		for (const auto& cell : openSet)
		{
			if (cell->FCost() < currentCell->FCost() || (cell->FCost() == currentCell->FCost() && cell->HCost < currentCell->HCost))
			{
				currentCell = cell;
			}
		}

		openSet.Remove(currentCell);
		closedSet.Add(currentCell);

		if (currentCell == targetCell)
		{
			while (currentCell != startCell)
			{
				outPath.CellsInPath.Add(currentCell);
				outPath.CellCosts.Add(currentCell->GCost);
				currentCell = GridCells[currentCell->ParentIndex];
			}
			
			Algo::Reverse(outPath.CellsInPath);
			Algo::Reverse(outPath.CellCosts);
			for (auto& cell : searchedCells) cell->GCost = 0;
			return true;
		}

		for (auto& cell : currentCell->GetNeighbors(CanMoveOnDiagonals, CanMoveVertically))
		{
			if (cell->State == ECellState::BLOCKED || closedSet.Contains(cell)) continue;

			float newGCost = currentCell->GCost + GetDistanceBetweenCells(currentCell, cell) + cell->MoveCost;
			if (!openSet.Contains(cell) || cell->GCost > newGCost)
			{
				cell->GCost = newGCost;
				cell->HCost = GetDistanceBetweenCells(cell, targetCell);
				cell->ParentIndex = currentCell->Index;

				if (!openSet.Contains(cell))
				{
					openSet.Add(cell);
					searchedCells.Add(cell);
				}
			}
		}
	}

	return false;
}

bool AGridManager::FindPathByCoordinate(FPath& outPath, const FIntVector& start, const FIntVector& end)
{

	UCell* startCell = GetCellFromCoordinates(start.X, start.Y, start.Z);
	if (!startCell)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start location not valid!"));
		return false;
	}

	UCell* targetCell = GetCellFromCoordinates(end.X, end.Y, end.Z);
	if (!targetCell)
	{
		UE_LOG(LogTemp, Warning, TEXT("End location not valid!"));
		return false;
	}

	return FindPathByCell(outPath, startCell, targetCell);
}

bool AGridManager::FindPathByLocation(FPath& outPath, const FVector& start, const FVector& end)
{
	UCell* startCell = GetClosestCellFromLocation(start);
	if (!startCell)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start location not valid!"));
		return false;
	}

	UCell* targetCell = GetClosestCellFromLocation(end);
	if (!targetCell)
	{
		UE_LOG(LogTemp, Warning, TEXT("End location not valid!"));
		return false;
	}
	return FindPathByCell(outPath, startCell, targetCell);
}


// Sets default values
AGridManager::AGridManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetRelativeLocation(FVector(0));
	CollisionBox->SetCollisionProfileName("Tile");
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGridManager::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AGridManager::OnOverlapEnd);
	SetRootComponent(CollisionBox);

	TileCheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Tile Checkbox"));
	TileCheckBox->SetCollisionProfileName("Tile");
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	TileCheckBox->SetBoxExtent(FVector(CellSize.X / 2.2f, CellSize.Y / 2.2f, CellSize.Z / 2.2f));
	CalculateSizes();
	CreateCells();
	CalculateGridBoxLines();
	InitializeModifierVolumes();
	SetAllCellNeighbors();
	SetPlayerControllerReferences();
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawGridBoxes();
}

