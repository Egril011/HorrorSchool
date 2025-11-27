// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "HorrorSchool/Interact/Interfaces/InteractInterface.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
}

void UInteractComponent::Interact(FVector StartPoint, FRotator Rotator)
{
	if (LineTrace(StartPoint, Rotator))
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor))
			return;
		
		if (HitActor->Implements<UInteractInterface>())
			IInteractInterface::Execute_Interactable(HitActor, GetOwner());
	}
}

void UInteractComponent::InteractHold(FVector StartPoint, FRotator Rotator, float HoldTime)
{
	if (LineTrace(StartPoint, Rotator))
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor))
			return;
		
		if (HitActor->Implements<UInteractInterface>())
			IInteractInterface::Execute_InteractableHold(HitActor, GetOwner(), HoldTime);
	}
}

bool UInteractComponent::LineTrace(FVector StartPoint, FRotator Rotator)
{
	FVector InteractLocation = StartPoint + (Rotator.Vector() * InteractDistance);

	if (!IsValid(GetWorld()))
		return false;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPoint,
		InteractLocation,
		ECC_Visibility);

	DrawDebugLine(
		GetWorld(),
		StartPoint,
		InteractLocation,
		FColor::Blue,
		true,
		10.f,
		0);
	
	return bHit;
}
