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
}

EBTNodeResult::Type UBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComponent = &OwnerComp;
	AHorrorEnemyAIController* HorrorEnemyAIController = Cast<AHorrorEnemyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(HorrorEnemyAIController))
		return EBTNodeResult::Failed;

	// Set the Max speed of the Enemy
	AAIHorrorEnemy* HorrorEnemy = Cast<AAIHorrorEnemy>(HorrorEnemyAIController->GetPawn());
	if (!IsValid(HorrorEnemy))
		return EBTNodeResult::Failed;

	UCharacterMovementComponent* CharacterMovementComponent = HorrorEnemy->GetCharacterMovement();
	if (!IsValid(CharacterMovementComponent))
		return EBTNodeResult::Failed;

	CharacterMovementComponent->MaxWalkSpeed = MoveSpeed;
	
	//Get the patrol point from the current index and move the AI toward it 
	CurrentIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("CurrentIndexPatrolPoint"));
	if (CurrentIndex < 0)
		return EBTNodeResult::Failed;
	
	APatrolPoint* PatrolPoints = HorrorEnemyAIController->PatrolPointsAI[CurrentIndex];

	HorrorEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	HorrorEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this,
		&UBTTaskNode_Patrol::OnMoveToCompleted);
	
	HorrorEnemyAIController->MoveToActor(PatrolPoints);
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTaskNode_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AHorrorEnemyAIController* HorrorEnemyAIController = Cast<AHorrorEnemyAIController>(
		OwnerComp.GetAIOwner()))
	{
		HorrorEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	}
	
	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Aborted;
}

void UBTTaskNode_Patrol::OnMoveToCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(BehaviorTreeComponent))
		return;

	AHorrorEnemyAIController* AIEnemyController = Cast<AHorrorEnemyAIController>(BehaviorTreeComponent->GetAIOwner());
	if (!IsValid(AIEnemyController))
		return;
	
	AIEnemyController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);

	// Advance to next patrol point or loop back to the beginning if it's a success
	if (Result.Code == EBTNodeResult::Succeeded)
	{
		CurrentIndex = (CurrentIndex + 1) % AIEnemyController->PatrolPointsAI.Num();  
		BehaviorTreeComponent->GetBlackboardComponent()->SetValueAsInt("CurrentIndexPatrolPoint", CurrentIndex);
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
	}

	FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Failed);
}
