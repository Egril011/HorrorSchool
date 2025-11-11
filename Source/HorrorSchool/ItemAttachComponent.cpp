// Copyright Epic Games, Inc. All Rights Reserved.


#include "ItemAttachComponent.h"
#include "HorrorSchoolCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UItemAttachComponent::UItemAttachComponent()
{
}

bool UItemAttachComponent::AttachItemToPlayer(AHorrorSchoolCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	
	// Check that the character is valid, and has no item yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UItemAttachComponent>())
		return false;
	
	// Attach the item to the Character
	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->AttachToComponent(Character->GetItemAttachment(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		
		return true;
	}
	
	return false;
}