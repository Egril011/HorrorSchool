// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class IInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interactable(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractableHold(AActor* Interactor, float HoldTime);
};