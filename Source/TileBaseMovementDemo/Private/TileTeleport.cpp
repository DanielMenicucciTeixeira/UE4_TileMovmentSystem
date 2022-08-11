// Fill out your copyright notice in the Description page of Project Settings.


#include "TileTeleport.h"
#include "TeleportBoxCollision.h"
#include "Cell.h"
#include "Components/BoxComponent.h"


// Sets default values
ATileTeleport::ATileTeleport()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Entry = CreateDefaultSubobject<UTeleportBoxCollision>(TEXT("Entry"));
	Entry->AttachTo(RootComponent);
	Exit = CreateDefaultSubobject<UTeleportBoxCollision>(TEXT("Exit"));
	Exit->AttachTo(RootComponent);
}

void ATileTeleport::SetComponentsData()
{
	if (GridManager)
	{
		
		UTeleportBoxCollision* teleportBox;
		for (auto& collider : GetComponents())
		{
			teleportBox = Cast<UTeleportBoxCollision>(collider);
			if (teleportBox)
			{
				UCell* cell = GridManager->GetClosestCellFromLocation(teleportBox->GetComponentLocation());
				teleportBox->SetWorldLocation(cell->Location);
				teleportBox->AffectedCell = cell;
			}
		}
		if (Entry->AffectedCell) UE_LOG(LogTemp, Warning, TEXT("Entry is valid"));
		if (Exit->AffectedCell) UE_LOG(LogTemp, Warning, TEXT("Exit is valid"));

		if (Entry->AffectedCell && Exit->AffectedCell)
		{
			Entry->AffectedCell->Neighbors.Add(Exit->AffectedCell);
			if(TwoWayTeleporter) Exit->AffectedCell->Neighbors.Add(Entry->AffectedCell);
		}
	}
}

// Called when the game starts or when spawned
void ATileTeleport::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATileTeleport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

