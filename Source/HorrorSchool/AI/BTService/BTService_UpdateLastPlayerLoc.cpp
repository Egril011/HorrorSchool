// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateLastPlayerLoc.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateLastPlayerLoc::UBTService_UpdateLastPlayerLoc()
{
	NodeName = "UpdateLastPlayerLoc";
	Interval = 2.f;
	bNotifyBecomeRelevant = true;
}

void UBTService_UpdateLastPlayerLoc::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Save the player location each Tick when the AI is seeing it
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player"))); 
	if (!IsValid(Player))
		return;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("LastPlayerLocation"), 
		Player->GetActorLocation());
}