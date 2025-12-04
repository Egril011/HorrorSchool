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
	bNotifyTaskFinished = true;
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
	
	if (UPathFollowingComponent* PFC = HorrorEnemyAIController->GetPathFollowingComponent())
	{
		PFC->OnRequestFinished.RemoveAll(this);
		PFC->OnRequestFinished.AddUObject(this, &UBTT_MoveToHeardLocation::OnMoveToComplete);
	}
	UE_LOG(LogTemp, Warning, TEXT("TESt8"));
	//Find if the hearing location is reachable
	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
		GetWorld(),
		CurrentPawn->GetActorLocation(),
		HearingLocation
		);

	// If reachable move the AI to this point otherwise find the nearest point
	if (NavigationPath && NavigationPath->IsValid() && !NavigationPath->IsPartial())
	{
		HorrorEnemyAIController->MoveToLocation(HearingLocation);
		return EBTNodeResult::InProgress;
	}
	
	if (HorrorEnemyAIController->FindNearestPoint(HearingLocation))
	{UE_LOG(LogTemp, Warning, TEXT("TESt7"));
		HorrorEnemyAIController->MoveToLocation(HearingLocation);
		return EBTNodeResult::InProgress;
	}
	UE_LOG(LogTemp, Warning, TEXT("TESt6"));
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTT_MoveToHeardLocation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IsValid(HorrorEnemyAIController))
	{ 
		if (UPathFollowingComponent* PFC = HorrorEnemyAIController->GetPathFollowingComponent())
		{
			PFC->OnRequestFinished.RemoveAll(this);
		}

		HorrorEnemyAIController->StopMovement();
		HorrorEnemyAIController = nullptr;
	}
	
	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Aborted;
}

void UBTT_MoveToHeardLocation::OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(BTComp))
		return;
	
	if (IsValid(HorrorEnemyAIController))
	{	
		if (UPathFollowingComponent* PFC = HorrorEnemyAIController->GetPathFollowingComponent())
		{
			PFC->OnRequestFinished.RemoveAll(this);
		}
		
		HorrorEnemyAIController->StopMovement();
		HorrorEnemyAIController = nullptr;
	}
	
	BTComp->GetBlackboardComponent()->ClearValue("HearingLocation");
	BTComp->GetBlackboardComponent()->SetValueAsBool(TEXT("BHearing"), false);
	
	if (Result.Code == EPathFollowingResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("TESt"));
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TESt1"));
		FinishLatentTask(*BTComp, EBTNodeResult::Failed);
	}
}