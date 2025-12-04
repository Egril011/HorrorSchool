// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateLastPlayerLoc.generated.h"

/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UBTService_UpdateLastPlayerLoc : public UBTService
{
	GENERATED_BODY()
	UBTService_UpdateLastPlayerLoc();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
