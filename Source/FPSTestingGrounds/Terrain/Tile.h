// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

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
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500.0f);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	bool CanSpawnAtLocation(FVector Location, float Radius);

	bool TryGetEmptyLocation(FVector &OutHitLocation, float Radius);

	//Helper function to Spawn Actor, set its location to Relative and attach it to the parent tile
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint);



	
	
};
