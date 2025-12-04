// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Patrol.generated.h"

class AHorrorEnemyAIController;
struct FPathFollowingResult;
class APatrolPoint;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()
	UBTTaskNode_Patrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnMoveToCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result);

	UPROPERTY(EditAnywhere, Category="PatrolPath|Variables")
	float MoveSpeed = 500.0f;
	
private:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY()
	TObjectPtr<AHorrorEnemyAIController> AIController;
	
	int CurrentIndex;
};
