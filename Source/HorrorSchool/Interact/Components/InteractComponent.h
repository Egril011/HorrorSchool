// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORSCHOOL_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

	//Execute the InteractInterface which is held by the Actor
	void Interact(FVector StartPoint, FRotator Rotator);
	void InteractHold(FVector StartPoint, FRotator Rotator, float HoldTime);

private:
	UPROPERTY(EditAnywhere, Category = "Interact|Variable", meta=(AllowPrivateAccess))
	float InteractDistance = 500.f;
	
	FHitResult HitResult;

	bool LineTrace(FVector StartPoint, FRotator Rotator);
};
