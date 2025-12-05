// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "HorrorSchool/Interact/Interfaces/InteractInterface.h"
#include "HorrorSchool/Player/PlayerControllerHorrorSchool.h"

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

void UInteractComponent::InteractTick(FVector StartPoint, FRotator Rotator)
{
	AActor* HitActor = nullptr;
	
	if (LineTrace(StartPoint, Rotator))
	{
		HitActor = HitResult.GetActor();
	}
	
	bool bIsInteactable = (IsValid(HitActor) && HitActor->Implements<UInteractInterface>());
	UpdateInteractUI(bIsInteactable);
}

void UInteractComponent::InteractHold(FVector StartPoint, FRotator Rotator, float HoldTime)
{
	AActor* NewHitActor = nullptr;
	
	if (LineTrace(StartPoint, Rotator))
	{
		NewHitActor = HitResult.GetActor();
	}
	
	if (CurrentInteractActor && CurrentInteractActor != NewHitActor)
	{
		if (CurrentInteractActor->Implements<UInteractInterface>())
		{
			IInteractInterface::Execute_InteractableHold(CurrentInteractActor, GetOwner(), 0.f);
		}
		CurrentInteractActor = nullptr;
	}
	
	if (IsValid(NewHitActor) && NewHitActor->Implements<UInteractInterface>())
	{
		CurrentInteractActor = NewHitActor;
		IInteractInterface::Execute_InteractableHold(CurrentInteractActor, GetOwner(), HoldTime);
	}
}

void UInteractComponent::InteractRelease()
{
	if (CurrentInteractActor && CurrentInteractActor->Implements<UInteractInterface>())
	{
		IInteractInterface::Execute_InteractableRelease(CurrentInteractActor, GetOwner());
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

void UInteractComponent::UpdateInteractUI(bool bInteractable)
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!PawnOwner)
	{
		return;
	}
	
	if (APlayerControllerHorrorSchool* ControllerHorrorSchool =
		Cast<APlayerControllerHorrorSchool>(PawnOwner->GetController()))
	{
		ControllerHorrorSchool->Interact(bInteractable);
	}
}