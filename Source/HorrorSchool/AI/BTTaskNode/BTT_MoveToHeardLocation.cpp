// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveToHeardLocation.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorSchool/AI/HorrorEnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_MoveToHeardLocation::UBTT_MoveToHeardLocation()
{
	NodeName = TEXT("MoveToHeardLocation");
	bCreateNodeInstance = true;
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
	
	// Modify the speed
	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(CurrentPawn->GetMovementComponent());
	if (!IsValid(CharacterMovementComponent))
		return EBTNodeResult::Failed;
	
	CharacterMovementComponent->MaxWalkSpeed = MoveSpeed;
	
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
	
	if (HorrorEnemyAIController->FindNearestPoint(HearingLocation))
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
	
	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Aborted;
}

void UBTT_MoveToHeardLocation::OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (IsValid(HorrorEnemyAIController))
	{	
		HorrorEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		HorrorEnemyAIController = nullptr;
	}
	
	if (!IsValid(BTComp))
		return;
	
	if (Result.Code == EPathFollowingResult::Success)
	{
		BTComp->GetBlackboardComponent()->ClearValue("HearingLocation");
		BTComp->GetBlackboardComponent()->SetValueAsBool(TEXT("BHearing"), false);
		
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
		return;
	}
	
	FinishLatentTask(*BTComp, EBTNodeResult::Failed);
}