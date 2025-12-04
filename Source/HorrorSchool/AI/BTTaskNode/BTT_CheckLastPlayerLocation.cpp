// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CheckLastPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HorrorSchool/AI/HorrorEnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_CheckLastPlayerLocation::UBTT_CheckLastPlayerLocation()
{
	NodeName = "Check Last Player Location";
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_CheckLastPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorComp = &OwnerComp;
	
	//Get the last player location
	FVector LastPlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("LastPlayerLocation"));
	if (LastPlayerLocation.Equals(FVector::ZeroVector))
		return EBTNodeResult::Failed;
	
	AIController = Cast<AHorrorEnemyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(AIController))
		return EBTNodeResult::Failed;
	
	if (UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
	{
		PFC->OnRequestFinished.RemoveAll(this);
		PFC->OnRequestFinished.AddUObject(this, &UBTT_CheckLastPlayerLocation::OnMoveToComplete);
	}
	
	//Move the AI to the last the AI saw the player
	AIController->MoveToLocation(LastPlayerLocation);
	return  EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_CheckLastPlayerLocation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IsValid(AIController))
	{
		if (UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
		{
			PFC->OnRequestFinished.RemoveAll(this);
		}
		
		AIController->StopMovement();
		AIController = nullptr;
	}
	
	BehaviorComp = nullptr;
	OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("LastPlayerLocation"));
	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Aborted;
}

void UBTT_CheckLastPlayerLocation::OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(BehaviorComp))
		return;
	
	if (IsValid(AIController))
	{
		if (UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
		{
			PFC->OnRequestFinished.RemoveAll(this);
		}
		AIController->StopMovement();
		AIController = nullptr;
	}
	
	if (Result.Code == EPathFollowingResult::Success)
	{
		BehaviorComp->GetBlackboardComponent()->ClearValue(TEXT("LastPlayerLocation"));
		FinishLatentTask(*BehaviorComp, EBTNodeResult::Succeeded);
		BehaviorComp = nullptr;
	}
	else
	{
		FinishLatentTask(*BehaviorComp, EBTNodeResult::Failed);
		BehaviorComp = nullptr;
	}
}
