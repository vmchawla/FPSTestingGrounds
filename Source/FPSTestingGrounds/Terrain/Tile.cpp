// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"


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



// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::SetPool(UActorPool * InPool)
{
	if (!InPool)
	{
		UE_LOG(LogTemp, Warning, TEXT("UActorPool instance is nullptr when setting local tile variable Pool"));
		return;
	}
	Pool = InPool;
	//UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()));

	PositionAndCheckoutNavMeshBoundsVolume();

}

void ATile::PositionAndCheckoutNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (!NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("NavMeshBoundsVolumePool is nullptr in Tile.cpp PositionandCheckoutNavMeshBoundsVolume()"));
		return;
	}
	
	//UE_LOG(LogTemp, Error, TEXT("[%s] Checkout: %s"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

template<class T>
void ATile::RandomyPlaceActors(TSubclassOf<T> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	int32 numberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (int32 i = 0; i < numberToSpawn; i++)
	{
		FVector SpawnPoint;
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		bool didGetEmptyLocation = TryGetEmptyLocation(SpawnPoint, Radius * RandomScale);
		float RandomYaw = FMath::RandRange(-181.0f, 181.0f);
		FSpawnPosition MySpawnPosition;
		MySpawnPosition.SpawnLocation = SpawnPoint;
		MySpawnPosition.Scale = RandomScale;
		MySpawnPosition.YawRotation = RandomYaw;

		if (didGetEmptyLocation)
		{
			PlaceActor(ToSpawn, MySpawnPosition);
		}
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	RandomyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius)
{
	RandomyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1);
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (!Spawned)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Spawned is null in Tile.cpp"));
		return;
	}
	Spawned->SetActorRelativeLocation(SpawnPosition.SpawnLocation);
	Spawned->SetActorRotation(FRotator(0.0f, SpawnPosition.YawRotation, 0.0f));
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(ToSpawn);
	if (!SpawnedPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIPawn Spawned is null in Tile.cpp"));
		return;
	}
	SpawnedPawn->SetActorRelativeLocation(SpawnPosition.SpawnLocation);
	SpawnedPawn->SetActorRotation(FRotator(0.0f, SpawnPosition.YawRotation, 0.0f));
	SpawnedPawn->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	SpawnedPawn->SpawnDefaultController();
	SpawnedPawn->Tags.Add(FName("Enemy"));
}


bool ATile::TryGetEmptyLocation(FVector &OutHitLocation, float Radius)
{
	FBox Bounds(MinExtent, MaxExtent);
	const int MAX_ATTEMPTS = 5;

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

	return !HasHit;
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("NavMeshBoundsVolumePool is nullptr in Tile.cpp EndPlay"));
		return;
	}
	Pool->Return(NavMeshBoundsVolume);

}


