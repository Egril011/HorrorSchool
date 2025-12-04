// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Patrol.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorSchool/HorrorSchoolCharacter.h"
#include "HorrorSchool/AI/AIHorrorEnemy.h"
#include "HorrorSchool/AI/HorrorEnemyAIController.h"
#include "HorrorSchool/AI/PatrolPoint/PatrolPoint.h"
#include "Navigation/PathFollowingComponent.h"

UBTTaskNode_Patrol::UBTTaskNode_Patrol()
{
	NodeName = "AI Patrol";
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComponent = &OwnerComp;
	
	AIController = Cast<AHorrorEnemyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(AIController))
		return EBTNodeResult::Failed;
	
	if (UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
	{
		PFC->OnRequestFinished.RemoveAll(this);
		PFC->OnRequestFinished.AddUObject(this, &UBTTaskNode_Patrol::OnMoveToCompleted);
	}
	
	// Set the Max speed of the Enemy
	AAIHorrorEnemy* HorrorEnemy = Cast<AAIHorrorEnemy>(AIController->GetPawn());
	if (!IsValid(HorrorEnemy))
		return EBTNodeResult::Failed;

	//Modify the Speed of the AI
	UCharacterMovementComponent* CharacterMovementComponent = HorrorEnemy->GetCharacterMovement();
	if (!IsValid(CharacterMovementComponent))
		return EBTNodeResult::Failed;

	CharacterMovementComponent->MaxWalkSpeed = MoveSpeed;
	
	//Get the patrol point from the current index and move the AI toward it 
	CurrentIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("CurrentIndexPatrolPoint"));
	if (CurrentIndex < 0)
		return EBTNodeResult::Failed;
	
	APatrolPoint* PatrolPoints = AIController->PatrolPointsAI[CurrentIndex];
	
	AIController->MoveToActor(PatrolPoints);
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTaskNode_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IsValid(AIController))
	{
		if (UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
		{
			PFC->OnRequestFinished.RemoveAll(this);
		}
		
		AIController->StopMovement();
		AIController = nullptr;
	}
	
	BehaviorTreeComponent = nullptr;
	
	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Aborted;
}

void UBTTaskNode_Patrol::OnMoveToCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(BehaviorTreeComponent))
		return;

	if (IsValid(AIController))
	{
		if (UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
		{
			PFC->OnRequestFinished.RemoveAll(this);
		}
		
		AIController->StopMovement();
	}

	// Advance to next patrol point or loop back to the beginning if it's a success
	if (Result.Code == EPathFollowingResult::Success)
	{
		CurrentIndex = (CurrentIndex + 1) % AIController->PatrolPointsAI.Num();  
		BehaviorTreeComponent->GetBlackboardComponent()->SetValueAsInt("CurrentIndexPatrolPoint", CurrentIndex);
		
		AIController = nullptr;
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
		BehaviorTreeComponent = nullptr;
	}
	else
	{
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Failed);
		BehaviorTreeComponent = nullptr;
	}
}
