// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"


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

	//CastSphere(GetActorLocation(), 300);

	//CastSphere(GetActorLocation() + FVector(0, 0, 1000.0f), 300);
	
	
}



// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{


	int numberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (int32 i = 0; i < numberToSpawn; i++)
	{
		FVector SpawnPoint;
		if (TryGetEmptyLocation(SpawnPoint, Radius))
		{
			PlaceActor(ToSpawn, SpawnPoint);
		}
		
		//FVector Spawnpoint = FMath::RandPointInBox(Bounds);
		////UE_LOG(LogTemp, Warning, TEXT("SpawnPoint: %s"), *(Spawnpoint.ToCompactString()));

		//AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
		//Spawned->SetActorRelativeLocation(Spawnpoint);
		//Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}


}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPoint);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

}

bool ATile::TryGetEmptyLocation(FVector &OutHitLocation, float Radius)
{
	FVector Min(0, -2000, 0);
	FVector Max(4000, 2000, 0);
	FBox Bounds(Min, Max);
	const int MAX_ATTEMPTS = 100;

	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);

		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutHitLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;

	//UE_LOG(LogTemp, Warning, TEXT("CanSpawnAtLocation called on Tile: %s"), *(GetName()));

	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel2, 
		FCollisionShape::MakeSphere(Radius)
	);

	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;  //A ? B: C If A is True than return B, else C

	DrawDebugSphere(GetWorld(), GlobalLocation, Radius, 20, ResultColor, true, 1000.0f);

	return !HasHit;
}



