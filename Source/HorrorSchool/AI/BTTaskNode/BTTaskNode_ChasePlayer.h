// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UBTTaskNode_ChasePlayer : public UBTTaskNode
{
	GENERATED_BODY()
	UBTTaskNode_ChasePlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComp;	
};
