// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_IsNoiseInRoom.generated.h"

namespace EPathFollowingRequestResult
{
	enum Type : int;
}

struct FPathFollowingResult;
class ADoor;
class AHorrorEnemyAIController;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UBTT_IsNoiseInRoom : public UBTTaskNode
{ 
	GENERATED_BODY()
	UBTT_IsNoiseInRoom();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult& Result);
	
	UPROPERTY(EditAnywhere, Category = "IsNoiseInRoom|Variables")
	float MoveSpeed = 500.f;
	
private:
	TObjectPtr<ADoor> TargetDoor = nullptr;
	TObjectPtr<AHorrorEnemyAIController> AIController = nullptr;
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent = nullptr;
};
