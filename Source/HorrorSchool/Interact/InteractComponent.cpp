// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "InteractInterface.h"
#include "LevelInstance/LevelInstanceTypes.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
}

void UInteractComponent::Interact(FVector StartPoint, FRotator Rotator) const
{
	FVector InteractLocation = StartPoint + (Rotator.Vector() * InteractDistance);
	FHitResult HitResult;
	if (!GetWorld())
		return;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPoint,
		InteractLocation,
		ECC_Visibility);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor))
			return;

		if (HitActor->Implements<UInteractInterface>())
			IInteractInterface::Execute_Interactable(HitActor, GetOwner());
	}
}
