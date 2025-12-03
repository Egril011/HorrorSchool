// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Interact.h"

#include "AIController.h"
#include "Interact/Interfaces/InteractInterface.h"

UBTT_Interact::UBTT_Interact()
{
	NodeName = "Interact";
}

EBTNodeResult::Type UBTT_Interact::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FHitResult Result;
	
	// Get the Pawn
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(CurrentPawn))
		return EBTNodeResult::Failed;
	
	// Make a lineTrace
	FVector EndLineTrace = CurrentPawn->GetActorLocation() + (CurrentPawn->GetActorRotation().Vector() *
		LineTraceDistance);
	
	if (!IsValid(GetWorld()))
		return EBTNodeResult::Failed;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Result, 
		CurrentPawn->GetActorLocation(),
		EndLineTrace,
		ECC_Visibility);
	
	DrawDebugLine(
		GetWorld(),
		CurrentPawn->GetActorLocation(),
		EndLineTrace,
		FColor::Red,
		true,
		50.0,
		0);
	
	if (bHit)
	{
		AActor* HitActor = Result.GetActor();
		if (!IsValid(HitActor))
			return EBTNodeResult::Failed;
		
		if (HitActor->Implements<UInteractInterface>())
		{
			IInteractInterface::Execute_Interactable(HitActor, CurrentPawn);
			return EBTNodeResult::Succeeded;
		}
		
		return	EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
