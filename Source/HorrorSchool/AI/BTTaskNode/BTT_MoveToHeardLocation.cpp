// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveToHeardLocation.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HorrorSchool/AI/HorrorEnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_MoveToHeardLocation::UBTT_MoveToHeardLocation()
{
	NodeName = TEXT("MoveToHeardLocation");
}

EBTNodeResult::Type UBTT_MoveToHeardLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(GetWorld()))
		return EBTNodeResult::Failed;

	BTComp = &OwnerComp;

	HearingLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("HearingLocation"));

	HorrorEnemyAIController = Cast<AHorrorEnemyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(HorrorEnemyAIController))
		return EBTNodeResult::Failed;

	APawn* CurrentPawn = HorrorEnemyAIController->GetPawn();
	if (!IsValid(CurrentPawn))
		return EBTNodeResult::Failed;
	
	//Find if the hearing location is reachable
	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
		GetWorld(),
		CurrentPawn->GetActorLocation(),
		HearingLocation
		);

	// If reachable move the AI to this point otherwise find the nearest point
	if (NavigationPath && NavigationPath->IsValid() && !NavigationPath->IsPartial())
	{
		HorrorEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		HorrorEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this,
			&UBTT_MoveToHeardLocation::OnMoveToComplete);
		HorrorEnemyAIController->MoveToLocation(HearingLocation);
		return EBTNodeResult::InProgress;
	}

	if (FindNearestPoint(CurrentPawn))
	{
		HorrorEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		HorrorEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this,
			&UBTT_MoveToHeardLocation::OnMoveToComplete);

		HorrorEnemyAIController->MoveToLocation(HearingLocation);
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTT_MoveToHeardLocation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IsValid(HorrorEnemyAIController))
	{
		HorrorEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		HorrorEnemyAIController = nullptr;
	}

	BTComp = nullptr;
	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Aborted;
}

void UBTT_MoveToHeardLocation::OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(BTComp))
		return;
	
	if (Result.Code == EPathFollowingResult::Success)
	{
		BTComp->GetBlackboardComponent()->ClearValue("HearingLocation");
		BTComp->GetBlackboardComponent()->SetValueAsBool(TEXT("BHearing"), false);
		
		if (IsValid(HorrorEnemyAIController))
		{
			HorrorEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
			HorrorEnemyAIController = nullptr;
		}
		
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
		return;
	}

	FinishLatentTask(*BTComp, EBTNodeResult::Failed);
}

bool UBTT_MoveToHeardLocation::FindNearestPoint(APawn* CurrentPawn)
{
	if (!IsValid(CurrentPawn) || !IsValid(GetWorld()))
		return false;
	  
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!IsValid(NavSys))
		return false;

	FNavLocation NavLocation;

	FVector Direction = (CurrentPawn->GetActorLocation() - HearingLocation).GetSafeNormal();
	FVector SearchLocation = HearingLocation;

	DrawDebugSphere(
		GetWorld(),
		SearchLocation,
		10.f,
		12,
		FColor::Red,
		false,
		50.f,
		0,
		1.f
	);
	
	float Step = StartingStep;
	
	//Searching the nearest point around the DotLocation 
	while (Step < MaxStep)
	{
		FVector Extend = FVector(Step, Step, Step);
		if (NavSys->ProjectPointToNavigation(SearchLocation, NavLocation, Extend))
		{
			HearingLocation = NavLocation.Location;
			DrawDebugSphere(
				GetWorld(),
				HearingLocation,
				10.f,
				12,
				FColor::Blue,
				false,
				50.f,
				0,
				1.f
	);
			return true;
		}

		// If not found increase by 2 
		Step *= 2;
	}
	
	return false;
}