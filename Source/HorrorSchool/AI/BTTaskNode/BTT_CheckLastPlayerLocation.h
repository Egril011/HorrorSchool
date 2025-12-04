// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CheckLastPlayerLocation.generated.h"

struct FPathFollowingResult;
class AHorrorEnemyAIController;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UBTT_CheckLastPlayerLocation : public UBTTaskNode
{
	GENERATED_BODY()
	UBTT_CheckLastPlayerLocation();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult& Result);
	
private:
	UPROPERTY()
	TObjectPtr<AHorrorEnemyAIController> AIController;
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorComp;
};
