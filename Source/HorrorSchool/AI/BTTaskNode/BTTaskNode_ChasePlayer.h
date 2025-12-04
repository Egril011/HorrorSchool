// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ChasePlayer.generated.h"

class AHorrorEnemyAIController;
struct FPathFollowingResult;
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
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult & Result);
	
	UPROPERTY(EditAnywhere, Category="ChasePlayer|Variables")
	float MoveSpeed = 800.f;

private:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorComp;
	
	UPROPERTY()
	TObjectPtr<AHorrorEnemyAIController> AIController;
};
