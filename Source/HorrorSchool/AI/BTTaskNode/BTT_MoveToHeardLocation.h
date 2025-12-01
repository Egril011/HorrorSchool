// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveToHeardLocation.generated.h"

class AHorrorEnemyAIController;
struct FPathFollowingResult;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UBTT_MoveToHeardLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_MoveToHeardLocation();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult& Result);

	// Maximum area limit from the center (example 800x800)
	UPROPERTY(EditAnywhere, Category = "MoveToHeardLocation|Variables")
	float MaxStep = 800.f;

	// The cube' size when the search is starting
	UPROPERTY(EditAnywhere, Category = "MoveToHeardLocation|Variables")
	float StartingStep = 50;

private:
	UPROPERTY()
	TObjectPtr<AHorrorEnemyAIController> HorrorEnemyAIController = nullptr;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BTComp = nullptr;

	FVector HearingLocation;

	bool FindNearestPoint(APawn* CurrentPawn);
};
