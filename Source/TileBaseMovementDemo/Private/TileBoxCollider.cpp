// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBoxCollider.h"

UTileBoxCollider::UTileBoxCollider()
{
	LineThickness = 2.0f;
	ShapeColor = FColor(0.002125f, 0.052861f, 0.0f, 1.0f);
	SetCollisionProfileName("TileCostVolume"); 
}

void UTileBoxCollider::Recalculate()
{
	if (!GridManager) return;
	USceneComponent::SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	BoxExtent.X = 0.5 * SizeX * GridManager->GetCellSize().X;
	BoxExtent.Y = 0.5 * SizeY * GridManager->GetCellSize().Y;
	BoxExtent.Z = 0.5 * SizeZ * GridManager->GetCellSize().Z;

	USceneComponent::SetRelativeLocation
	(
		FVector
		(
			PositionX * GridManager->GetCellSize().X,
			PositionY * GridManager->GetCellSize().Y,
			PositionZ * GridManager->GetCellSize().Z
		)
		+ (0.5 * FVector
		(
			(SizeX - 1) * GridManager->GetCellSize().X,
			(SizeY - 1) * GridManager->GetCellSize().Y,
			(SizeZ - 1) * GridManager->GetCellSize().Z
		))
		//+ FVector(0.0f, 0.0f, 0.5f * BoxExtent.Z)
	);
}

void UTileBoxCollider::RefreshAffectedCells()
{
	for (auto& cell : AffectedCells)
	{
		cell->SetCellParameters(State, GridManager->GetBaseMoveCost() + MoveCostModifier, Priority);
	}
}
