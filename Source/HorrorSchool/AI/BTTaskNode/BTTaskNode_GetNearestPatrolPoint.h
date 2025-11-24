// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_GetNearestPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UBTTaskNode_GetNearestPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	UBTTaskNode_GetNearestPatrolPoint();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
