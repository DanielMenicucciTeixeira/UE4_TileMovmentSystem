// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.h"
#include "TileBoxCollider.h"
#include "TileModifierVolume.generated.h"

UCLASS()
class TILEBASEMOVEMENTDEMO_API ATileModifierVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileModifierVolume();

	UFUNCTION(BlueprintCallable)
		virtual void SetComponentsData();

	inline void SetGridManager(AGridManager* gridManager) { GridManager = gridManager; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		AGridManager* GridManager = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
