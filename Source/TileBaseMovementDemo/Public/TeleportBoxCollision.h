// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TeleportBoxCollision.generated.h"

class UCell;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILEBASEMOVEMENTDEMO_API UTeleportBoxCollision : public USceneComponent
{
	GENERATED_BODY()

public:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCell* AffectedCell;
};
