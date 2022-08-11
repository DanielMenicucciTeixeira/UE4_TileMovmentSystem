// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Cell.generated.h"


UENUM()
enum ECellState
{
	FREE UMETA(DisplayName = "Free"),
	OCCUPIED UMETA(DisplayName = "Occupied"),
	BLOCKED UMETA(DisplayName = "Blocked"),
	DEAD UMETA(DisplayName = "Dead")
};

UCLASS(ClassGroup = (Custom), Blueprintable)
class TILEBASEMOVEMENTDEMO_API UCell : public UObject
{
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Grid")
		int32 Index = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Grid")
		FIntVector Coordinates;
	UPROPERTY(BlueprintReadOnly, Category = "Grid")
		FVector Location;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float MoveCost;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		TEnumAsByte<ECellState> State;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		TArray<AActor*> Occupants;

	UPROPERTY(BlueprintReadOnly, Category = "Pathfinding")
		float GCost = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Pathfinding")
		float HCost = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Pathfinding")
		int32 ParentIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Pathfinding")
		FColor Color;

	UPROPERTY(BlueprintReadWrite)
		int32 ModifierPriority = -999;
	
	TSet<UCell*> Neighbors;
	TSet<UCell*> DiagonalNeighbors;
	TSet<UCell*> Diagonal3DNeighbors;

	//Constructors
	UCell();
	UCell(UCell* cell);
	UCell(int32 x, int32 y, int32 z, int32 index, FVector location = FVector(0.0f, 0.0f, 0.0f), float moveCost = 0, TEnumAsByte<ECellState> state = ECellState::FREE);

	void SetColorByState();
	void SetCellParameters(TEnumAsByte<ECellState> state, float moveCost, int32 modifierPriority);
	void SetPathfindingData(float gCost, float hCost, int32 parentIndex);
	void SetPathfindingData(const UCell& cell);
	
	inline float FCost() const { return GCost + HCost; }

	TSet<UCell*> GetNeighbors(const bool& diagonal = false, const bool& vertical = false) const;

	inline bool operator== (const UCell& cell) const { return Index == cell.Index; }
	inline bool operator!= (const UCell& cell) const { return Index != cell.Index; }
	//void operator= (const UCell& cell);

	friend uint32 GetTypeHash(const UCell& cell)
	{
		return GetTypeHash(cell.Index);
	}
};
