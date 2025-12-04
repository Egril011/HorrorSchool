// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DistancePawnPlayer.generated.h"

/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UBTService_DistancePawnPlayer : public UBTService
{
	GENERATED_BODY()
	UBTService_DistancePawnPlayer();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category="DistancePawnPlayer|Variable")
	float DistanceToReach;
};
