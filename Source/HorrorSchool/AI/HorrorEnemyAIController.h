// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HorrorEnemyAIController.generated.h"

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
	AHorrorEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Sight> AISenseSight;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviourTree;

	UPROPERTY(VisibleAnywhere, Category="AI")
	UBlackboardComponent* BlackboardComponent;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
