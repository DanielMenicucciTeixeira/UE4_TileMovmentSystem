// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GridManager.h"
#include "TileBoxCollider.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILEBASEMOVEMENTDEMO_API UTileBoxCollider : public UBoxComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
		int32 SizeX = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
		int32 SizeY = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
		int32 SizeZ =1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
		int32 PositionX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
		int32 PositionY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
		int32 PositionZ = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MoveCostModifier = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		TEnumAsByte<ECellState> State;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Priority = 0;

	UTileBoxCollider();

	inline TEnumAsByte<ECellState> GetState() { return State; }
	inline float GetMoveCostModifier() { return MoveCostModifier; }

	TArray<UCell*> AffectedCells;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		AGridManager* GridManager;

	UFUNCTION(BlueprintCallable)
		void Recalculate();

	UFUNCTION(BlueprintCallable)
		void RefreshAffectedCells();
	
};
