// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

UCell::UCell()
{
	Coordinates = FIntVector();
	Location = FVector(0.0f, 0.0f, 0.0f);
	MoveCost = 1;
	State = ECellState::FREE;
	SetColorByState();
}

UCell::UCell(UCell* cell)
{
	Coordinates = cell->Coordinates;
	Index = cell->Index;
	Location = cell->Location;
	State = cell->State;
	MoveCost = cell->MoveCost;
	Color = cell->Color;
}

UCell::UCell(int32 x, int32 y, int32 z, int32 index, FVector location, float moveCost, TEnumAsByte<ECellState> state)
{
	Coordinates = FIntVector(x, y, z);
	Index = index;
	Location = location;
	State = state;
	MoveCost = moveCost;
	SetColorByState();
}

void UCell::SetColorByState()
{
	switch (State)
	{
	case FREE: Color = FColor::Green;
		break;
	case OCCUPIED: Color = FColor::Yellow;
		break;
	case BLOCKED: Color = FColor::Red;
		break;
	case DEAD: Color = FColor::Black;
		break;
	default: Color = FColor::White;
	}
}

void UCell::SetCellParameters(TEnumAsByte<ECellState> state, float moveCost, int32 modifierPriority)
{
	if (modifierPriority >= ModifierPriority)
	{
		ModifierPriority = modifierPriority;
		MoveCost = moveCost;
		State = state;
		SetColorByState();
	}
}

void UCell::SetPathfindingData(float gCost, float hCost, int32 parentIndex)
{
	GCost = gCost;
	HCost = hCost;
	ParentIndex = parentIndex;
}

void UCell::SetPathfindingData(const UCell& cell)
{
	GCost = cell.GCost;
	HCost = cell.HCost;
	ParentIndex = cell.ParentIndex;
}

TSet<UCell*> UCell::GetNeighbors(const bool& diagonal, const bool& vertical) const
{
	TSet<UCell*> neighbors;
	neighbors.Append(Neighbors);
	if (diagonal)
	{
		neighbors.Append(DiagonalNeighbors);
		if (vertical)
		{
			neighbors.Append(Diagonal3DNeighbors);
		}
	}

	return neighbors;
}

/*void UCell::operator=(const UCell& cell)
{
	Coordinates = cell.Coordinates;
	Location = cell.Location;
	MoveCost = cell.MoveCost;
	State = cell.State;
	Occupants = cell.Occupants;
	Color = cell.Color;

	Neighbors = cell.Neighbors;
	DiagonalNeighbors = cell.DiagonalNeighbors;
	Diagonal3DNeighbors = cell.Diagonal3DNeighbors;

	GCost = cell.GCost;
	HCost = cell.HCost;
}*/
