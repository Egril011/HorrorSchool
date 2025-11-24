// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_GetNearestPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HorrorSchool/AI/HorrorEnemyAIController.h"
#include "HorrorSchool/AI/PatrolPoint/PatrolPoint.h"

UBTTaskNode_GetNearestPatrolPoint::UBTTaskNode_GetNearestPatrolPoint()
{
	NodeName = "GetNearestPatrolPoint";
}

EBTNodeResult::Type UBTTaskNode_GetNearestPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(OwnerPawn))
		return EBTNodeResult::Failed;

	AHorrorEnemyAIController* HorrorEnemyAIController = Cast<AHorrorEnemyAIController>(
		OwnerComp.GetAIOwner());
	if (!IsValid(HorrorEnemyAIController))
		return EBTNodeResult::Failed;

	// Find the nearest patrol point for the Ai based on its location
	const FVector PawnLocation = OwnerPawn->GetActorLocation();
	float NearestPatrolPointLocation = HorrorEnemyAIController->PatrolPointsAI[0]->GetActorLocation().Size();
	int CurrentIndex = 0;

	for (int i = 0; i < HorrorEnemyAIController->PatrolPointsAI.Num(); i++)
	{
		const AActor* PatrolPoint = HorrorEnemyAIController->PatrolPointsAI[i];
		
		float Distance = FVector::Dist(PawnLocation, PatrolPoint->GetActorLocation());
		if (Distance < NearestPatrolPointLocation)
		{
			NearestPatrolPointLocation = Distance;
			CurrentIndex = i;
		}
	}

	//Save the CurrentIndex into the BlackBoard
	OwnerComp.GetBlackboardComponent()->SetValueAsInt("CurrentIndexPatrolPoint", CurrentIndex);
	return EBTNodeResult::Succeeded;
}