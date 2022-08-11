// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileModifierVolume.h"

#include "TileTeleport.generated.h"

class UTeleportBoxCollision;

UCLASS(Blueprintable)
class TILEBASEMOVEMENTDEMO_API ATileTeleport : public ATileModifierVolume
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileTeleport();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		UTeleportBoxCollision* Entry = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		UTeleportBoxCollision* Exit = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid")
		bool TwoWayTeleporter = true;

		void SetComponentsData() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
