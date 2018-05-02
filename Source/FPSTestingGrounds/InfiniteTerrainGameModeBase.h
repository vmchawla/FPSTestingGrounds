// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfiniteTerrainGameModeBase.generated.h"

//Forward Declarations
class ANavMeshBoundsVolume;
class UActorPool;

/**
 * 
 */
UCLASS()
class FPSTESTINGGROUNDS_API AInfiniteTerrainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInfiniteTerrainGameModeBase();

	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
	void PopulateBoundsVolume();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavMeshBoundsVolumePool")
	UActorPool* NavMeshActorPool;

private:
	void AddToPool(ANavMeshBoundsVolume *VolumeToAdd);

	
};
