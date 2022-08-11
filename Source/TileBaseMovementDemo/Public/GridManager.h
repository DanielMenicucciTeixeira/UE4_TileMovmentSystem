#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Color.h"
#include "Engine/World.h"
#include "Components/LineBatchComponent.h"
#include "Engine/DataTable.h"
#include "Math/IntVector.h"
#include "Cell.h"

#include "GridManager.generated.h"


class UBoxComponent;
class ATileMovementPlayerController;

USTRUCT(BlueprintType)
struct FPath
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UCell*> CellsInPath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<float> CellCosts;

	void Empty()
	{
		CellsInPath.Empty();
		CellCosts.Empty();
	}
};

UCLASS(ClassGroup = (Custom), Blueprintable)
class TILEBASEMOVEMENTDEMO_API AGridManager : public AActor
{
	GENERATED_BODY()
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
		FVector GridSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cells")
		FIntVector CellCount;
	UPROPERTY(EditAnywhere, Category = "Cells")
		FVector CellSize = FVector(100.0f, 100.0f, 100.0f);
	
	UPROPERTY(EditAnywhere, Category = "Movement")
		float BaseMoveCost = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		bool CanMoveOnDiagonals = false;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DiagonalCostMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		bool CanMoveVertically = false;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float Diagonal3DCostMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Lines")
		FLinearColor GridLineColor = FColor::Black;
	UPROPERTY(EditAnywhere, Category = "Lines")
		float GridLineThickness = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lines")
		bool DrawHorizontalLines = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lines")
		bool DrawVerticalLines = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		UBoxComponent* CollisionBox = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Grid")
		UBoxComponent* TileCheckBox = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cells")
	TArray<UCell*> GridCells;

	TArray<FBatchedLine> GridLines;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Grid")
	TArray<AActor*> TileModifierVolumes;

	UCell* GetCellFromCoordinates(int32 x, int32 y, int32 z) const;
	bool GetCellIndexFromGridPosition(int32& index, int32 x, int32 y, int32 z) const;
	void CollisionBoxSetUP();
	void SetPlayerControllerReferences();

public:
	UFUNCTION(BlueprintPure)
		inline float GetBaseMoveCost() { return BaseMoveCost; }
	UFUNCTION(BlueprintPure)
		inline FVector GetGridSize() { return GridSize; };
	UFUNCTION(BlueprintPure)
		inline FIntVector GetCellCount() { return CellCount; };
	UFUNCTION(BlueprintPure)
		inline TArray<UCell*> GetGridCells() { return GridCells; };
	UFUNCTION(BlueprintPure)
		inline FVector GetCellSize() { return CellSize; }

	UFUNCTION(BlueprintCallable)
		void SetGridSize(float x, float y, float z);
	UFUNCTION(BlueprintCallable)
		void SetCellCount(int32 x, int32 y, int32 z);
	UFUNCTION(BlueprintCallable)
		void SetCellSize(float x, float y, float z);

	UFUNCTION(BlueprintCallable)
		void DrawGridBoxes();
	UFUNCTION(BlueprintCallable)
		void CreateCells();
	UFUNCTION(BlueprintCallable)
		void CalculateGridBoxLines();
	UFUNCTION(BlueprintCallable)
		void CheckCellBlocks();
	UFUNCTION(BlueprintCallable)
		void CalculateSizes();
	UFUNCTION(BlueprintCallable)
		void SetCell(int32 cellIndex, TEnumAsByte<ECellState> state, float moveCost, int32 modifierPriority);

	UFUNCTION(BlueprintCallable)
		UCell* GetClosestCellFromLocation(const FVector& location) const;

	UFUNCTION(BlueprintCallable)
		float GetDistanceBetweenCells(const UCell* cellA, const UCell* cellB, const bool& diagonal = false, const bool& vertical = false) const;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void InitializeModifierVolumes();

	UFUNCTION(BlueprintCallable)
		void SetCellNeighbors(UCell* cell);
	UFUNCTION(BlueprintCallable)
		void SetAllCellNeighbors();

	UFUNCTION(BlueprintCallable)
		bool FindPathByCell(FPath& outPath, UCell* start, UCell* end);
	UFUNCTION(BlueprintCallable)
		bool FindPathByCoordinate(FPath& outPath, const FIntVector& start, const FIntVector& end);
	UFUNCTION(BlueprintCallable)
		bool FindPathByLocation(FPath& outPath, const FVector& start, const FVector& end);

public:
	// Sets default values for this actor's properties
	AGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		AActor* TestStart;

	UPROPERTY(EditAnywhere)
		AActor* TestEnd;

	TArray<UCell*> TestPath;
};
