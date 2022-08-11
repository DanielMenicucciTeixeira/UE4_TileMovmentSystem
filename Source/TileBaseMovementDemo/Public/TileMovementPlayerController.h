// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridManager.h"
#include "GameFramework/PlayerController.h"
#include "TileMovementPlayerController.generated.h"

class AGridManager;
class UCell;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMoveLimitColor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MoveLimit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor Color;
};

UCLASS(Blueprintable)
class TILEBASEMOVEMENTDEMO_API ATileMovementPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Characters")
		APawn* SelectedCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Grid")
		AGridManager* GridManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Characters")
		float MaximumMovement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathTracing")
		TArray<FMoveLimitColor> MovementColors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathTracing")
		FColor OverLimitColor = FColor::Red;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
		float DistanceToCellTolerance = 120.0f;

	UFUNCTION(BlueprintCallable, Category = "PathTracing")
		void DrawPathToCursor();

	UFUNCTION(BlueprintCallable, Category = "PathTracing")
		bool TracePathToCursor(FPath& outPath);

	UFUNCTION(BlueprintCallable, Category = "PathTracing")
		FColor ChooseCellColor(float moveCost);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetNewWalkPath();

	/** Navigate player to the given world location. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetNewMoveDestination(const FVector DestLocation);

	inline void SetGridManager(AGridManager* gridManager) { GridManager = gridManager; }

protected:

	FPath PathToDraw;
	FPath PathToWalk;
	bool Walk = false;
	int PathIndex = 0;

	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ATileMovementPlayerController();

};
