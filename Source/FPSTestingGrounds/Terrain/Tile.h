// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector SpawnLocation;
	float YawRotation;
	float Scale;

};


//Forward Declarations
class UActorPool;

UCLASS()
class FPSTESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, float Radius = 500.0f, float MinScale = 1, float MaxScale = 1);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, float Radius = 500.0f);


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Overridable function called whenever this actor is being removed from a level */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
		FVector NavigationBoundsOffset = FVector(2000.0, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
		FVector MinExtent = FVector(0.0f, -2000.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
		FVector MaxExtent = FVector(4000.0f, 2000.0f, 0.0f);

	//FVector Min(0, -2000, 0);
	//FVector Max(4000, 2000, 0);

public:

	//Set our local UActor Pool Variable and Position the NavMesh Bounds Volume
	UFUNCTION(BlueprintCallable, Category = "Pool")
		void SetPool(UActorPool* Pool);

private:

	template<class T>
	void RandomyPlaceActors(TSubclassOf<T> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, float Radius = 500.0f, float MinScale = 1, float MaxScale = 1);

	//Helper function for PlaceActors to Spawn Actor, set its Relative location and rotation and attach it to the parent tile
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);

	//Helper function for PlaceAIPawns to Spawn Actor, set its Relative location and rotation and attach it to the parent tile
	void PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition);

	bool TryGetEmptyLocation(FVector &OutHitLocation, float Radius);

	//Helper Function called in TryGetEmptyLocation
	bool CanSpawnAtLocation(FVector Location, float Radius);

	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;

	void PositionAndCheckoutNavMeshBoundsVolume();

};