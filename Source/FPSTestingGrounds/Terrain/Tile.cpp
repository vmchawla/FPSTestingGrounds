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

	//TActorIterator<AActor> ActorIterator = TActorIterator<AActor>(GetWorld());
	//while (ActorIterator)
	//{
	//	AActor* FoundActor = *ActorIterator;
	//	UE_LOG(LogTemp, Warning, TEXT("Found ACtor: %s"), *FoundActor->GetName());
	//	++ActorIterator;
	//}

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
	UE_LOG(LogTemp, Error, TEXT("[%s] Checkout: %s"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{


	int numberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (int32 i = 0; i < numberToSpawn; i++)
	{
		FVector SpawnPoint;
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		if (TryGetEmptyLocation(SpawnPoint, Radius * RandomScale ))
		{
			float RandomYaw = FMath::RandRange(-181.0f, 181.0f);
			
			PlaceActor(ToSpawn, SpawnPoint, RandomYaw, RandomScale);
		}
	}


}


void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Yaw, float Scale)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPoint);
	Spawned->SetActorRotation(FRotator(0.0f, Yaw, 0.0f));
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorScale3D(FVector(Scale));
	

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
	//if (!NavMeshBoundsVolume)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("NavMeshBoundsVolumePool is nullptr in Tile.cpp EndPlay"));
	//	return;
	//}
	Pool->Return(NavMeshBoundsVolume);

}


