// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


AActor * UActorPool::Checkout()
{ 
	if (Pool.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool had zero NavMeshVolumes when attempting to checkout. First Tile?"));
		return nullptr;
	}
	return Pool.Pop();
}

void UActorPool::Return(AActor* ActorToReturn)
{
	Add(ActorToReturn);
}

void UActorPool::Add(AActor * ActorToAdd)
{
	if (!ActorToAdd)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Actor added is null"), *(GetName()));
		return;
	}
	Pool.Push(ActorToAdd);
}

