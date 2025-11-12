// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "ItemAttachComponent.generated.h"

class AHorrorSchoolCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORSCHOOL_API UItemAttachComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties */
	UItemAttachComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachItemToPlayer(AHorrorSchoolCharacter* TargetCharacter);

private:
	/** The Character holding this weapon*/
	AHorrorSchoolCharacter* Character;
};
