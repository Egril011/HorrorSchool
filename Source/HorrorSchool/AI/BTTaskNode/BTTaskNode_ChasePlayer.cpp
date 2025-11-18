// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChasePlayer.h"

#include "AIController.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

UBTTaskNode_ChasePlayer::UBTTaskNode_ChasePlayer()
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type UBTTaskNode_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
		return EBTNodeResult::Failed;

	// Get the Player
	AActor* Player = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("Player")));
	if (!IsValid(Player))
		return EBTNodeResult::Failed;

	//Move the AI to the player
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
		return EBTNodeResult::Failed;
	
	AICon->MoveToActor(Player); 
	return EBTNodeResult::Succeeded;
}
