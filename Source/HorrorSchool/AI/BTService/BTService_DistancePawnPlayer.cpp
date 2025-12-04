// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DistancePawnPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_DistancePawnPlayer::UBTService_DistancePawnPlayer()
{
	NodeName = "Distance Pawn Player";
	Interval = 0.5f;
	bNotifyBecomeRelevant = true;
}

void UBTService_DistancePawnPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Get the Pawn
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(AIController))
		return;
	
	APawn* Pawn = AIController->GetPawn();
	if (!IsValid(Pawn))
		return;
	
	//Get the player
	APawn* Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
	if (!IsValid(Player))
		return;
	
	//Calcule the Distance
	float Distance = FVector::Distance(Pawn->GetActorLocation(), Player->GetActorLocation());
	bool bTouchesPlayer = Distance < DistanceToReach;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bTouchesPlayer"), bTouchesPlayer);
}
