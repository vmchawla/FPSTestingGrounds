// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRouteComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	//TODO protect against no Patrol Route component

	//TODO protect agaisnt empty patrol routes



	//Initialise local variable
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRouteComponent>();

	if (!PatrolRoute)
	{
		UE_LOG(LogTemp, Warning, TEXT("No patrol route component found. Forget to attack patrol route?"));
		return EBTNodeResult::Failed;
	}

	PatrolPoints = PatrolRoute->GetPatrolPoints();

	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No routes set on Patrol route component"));
		return EBTNodeResult::Failed;
	}

	//set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);


	//cycle index
	int32 modulo = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, modulo);

	return EBTNodeResult::Succeeded;
}

 