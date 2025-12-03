// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Interact.generated.h"

/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UBTT_Interact : public UBTTaskNode
{
	GENERATED_BODY()
	UBTT_Interact();
	
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category="Interact|Variables")
	float LineTraceDistance = 500.f;
	
};
