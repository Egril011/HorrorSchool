// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChasePlayer.h"

#include "AIController.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorSchool/AI/AIHorrorEnemy.h"
#include "HorrorSchool/AI/HorrorEnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"

UBTTaskNode_ChasePlayer::UBTTaskNode_ChasePlayer()
{
	NodeName = "Chase Player";
	bCreateNodeInstance = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTaskNode_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorComp = &OwnerComp;
	
	//Move the AI to the player
	AIController = Cast<AHorrorEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
		return EBTNodeResult::Failed;
	
	if (UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
	{
		PFC->OnRequestFinished.RemoveAll(this);
		PFC->OnRequestFinished.AddUObject(this, &UBTTaskNode_ChasePlayer::OnMoveToComplete);
	}
	
	AAIHorrorEnemy* AIHorrorEnemy = Cast<AAIHorrorEnemy>(AIController->GetPawn());
	if (!IsValid(AIHorrorEnemy))
		return EBTNodeResult::Failed;
	
	//Modify the Speed
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(AIHorrorEnemy->GetMovementComponent());
	if (!IsValid(CharacterMovement))
		return EBTNodeResult::Failed;
	
	CharacterMovement->MaxWalkSpeed = MoveSpeed;
	
	//Stop the Sound from the AI
	AIHorrorEnemy->StopSound();
	
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Player"));
	if (!IsValid(Player))
		return EBTNodeResult::Failed;
	
	AIController->MoveToActor(Player);
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTaskNode_ChasePlayer::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Aborted;
}

void UBTTaskNode_ChasePlayer::OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(BehaviorComp))
		return;
	
	if (IsValid(AIController))
	{
		if(UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
		{
			PFC->OnRequestFinished.RemoveAll(this);
		}
		
		AIController->StopMovement();
		AIController = nullptr;
	}
	
	if (Result.Code == EPathFollowingResult::Success)
	{
		FinishLatentTask(*BehaviorComp, EBTNodeResult::Succeeded);
		BehaviorComp = nullptr;
	}
	else
	{
		FinishLatentTask(*BehaviorComp, EBTNodeResult::Failed);
		BehaviorComp = nullptr;
	}
}