// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChasePlayer.h"

#include "AIController.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "HorrorSchool/AI/HorrorEnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"

UBTTaskNode_ChasePlayer::UBTTaskNode_ChasePlayer()
{
	NodeName = "Chase Player";
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTaskNode_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorComp = &OwnerComp;
	
	//Move the AI to the player
	AHorrorEnemyAIController* AICon = Cast<AHorrorEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AICon)
		return EBTNodeResult::Failed;
	
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Player"));
	if (!IsValid(Player))
		return EBTNodeResult::Failed;
	
	AICon->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	AICon->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this,
		&UBTTaskNode_ChasePlayer::OnMoveToComplete);
	
	AICon->MoveToActor(Player);
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTaskNode_ChasePlayer::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AHorrorEnemyAIController* AICon = Cast<AHorrorEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		AICon->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	}

	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Aborted;
}

void UBTTaskNode_ChasePlayer::OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(BehaviorComp))
		return;
	
	AHorrorEnemyAIController* AICon = Cast<AHorrorEnemyAIController>(BehaviorComp->GetAIOwner());
	if (!IsValid(AICon))
		return;

	AICon->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	
	if (Result.Code == EBTNodeResult::Failed || Result.Code == EBTNodeResult::Aborted)
	{
		FinishLatentTask(*BehaviorComp, EBTNodeResult::Failed);
	}
	
	FinishLatentTask(*BehaviorComp, EBTNodeResult::Succeeded);
}
