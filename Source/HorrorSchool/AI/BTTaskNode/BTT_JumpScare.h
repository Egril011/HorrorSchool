// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_JumpScare.generated.h"

class AHorrorSchoolCharacter;
class AAIHorrorEnemy;
class AHorrorEnemyAIController;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UBTT_JumpScare : public UBTTaskNode
{
	GENERATED_BODY()
	UBTT_JumpScare();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShake;
	
private:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY()
	TObjectPtr<AHorrorEnemyAIController> AIController;
	
	UPROPERTY()
	TObjectPtr<AAIHorrorEnemy> AIHorrorEnemy;
	
	UPROPERTY()
	TObjectPtr<AHorrorSchoolCharacter> Player;
	
	void MovePlayerCameraToPawnCamera();
};
