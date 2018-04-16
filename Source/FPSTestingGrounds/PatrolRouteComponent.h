// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolRouteComponent.generated.h"

//A Route card that helps AI choose next waypoint
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSTESTINGGROUNDS_API UPatrolRouteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's propertiess
	UPatrolRouteComponent();

	TArray<AActor*> GetPatrolPoints() const;

protected:
	UPROPERTY(EditInstanceOnly, Category = "Setup")
		TArray<AActor*> PatrolPoints;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
