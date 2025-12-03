// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HorrorEnemyAIController.generated.h"

class UAISenseConfig_Hearing;
class APatrolPoint;
struct FAIStimulus;
class UAISenseConfig_Sight;
class UBehaviorTree;
class UBlackboardComponent;
class UAIPerceptionComponent;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API AHorrorEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	
	AHorrorEnemyAIController();
	bool FindNearestPoint(FVector& ObjectLocation);
	
	UPROPERTY()
	TArray<APatrolPoint*> PatrolPointsAI;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Sight> AISenseSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Hearing> AISenseHearing;   

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;

	UPROPERTY(VisibleAnywhere, Category="AI")
	UBlackboardComponent* BlackboardComponent;
	
	// Maximum area limit from the center (example 800x800)
	UPROPERTY(EditAnywhere, Category = "MoveToHeardLocation|Variables")
	float MaxStep = 800.f;

	// The cube' size when the search is starting
	UPROPERTY(EditAnywhere, Category = "MoveToHeardLocation|Variables")
	float StartingStep = 50;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
