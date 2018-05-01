// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameModeBase.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"

AInfiniteTerrainGameModeBase::AInfiniteTerrainGameModeBase()
{
	NavMeshActorPool = CreateDefaultSubobject<UActorPool>(FName("NavMeshActorPool"));
}

void AInfiniteTerrainGameModeBase::PopulateBoundsVolume()
{
	for (TActorIterator<ANavMeshBoundsVolume> VolumeIterator (GetWorld()); VolumeIterator; ++VolumeIterator)
	{
		AddToPool(*VolumeIterator);
	}

}


void AInfiniteTerrainGameModeBase::AddToPool(ANavMeshBoundsVolume * VolumeToAdd)
{
	NavMeshActorPool->Add(VolumeToAdd);
}
