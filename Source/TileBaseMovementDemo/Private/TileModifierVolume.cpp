// Fill out your copyright notice in the Description page of Project Settings.


#include "TileModifierVolume.h"

// Sets default values
ATileModifierVolume::ATileModifierVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATileModifierVolume::SetComponentsData()
{
	if (GridManager)
	{
		for (auto& collider : GetComponents())
		{
			if (Cast<UTileBoxCollider>(collider))
			{
				Cast<UTileBoxCollider>(collider)->GridManager = GridManager;
				Cast<UTileBoxCollider>(collider)->Recalculate();
			}
		}
	}
}

// Called when the game starts or when spawned
void ATileModifierVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATileModifierVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

