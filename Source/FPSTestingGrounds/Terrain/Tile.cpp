// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn)
{
	FVector Min(0, -1900, 0);
	FVector Max(4000, 1900, 0);
	FBox Bounds(Min, Max);

	int numberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (int32 i = 0; i < numberToSpawn; i++)
	{
		FVector Spawnpoint = FMath::RandPointInBox(Bounds);
		//UE_LOG(LogTemp, Warning, TEXT("SpawnPoint: %s"), *(Spawnpoint.ToCompactString()));

		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
		Spawned->SetActorRelativeLocation(Spawnpoint);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}


}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

