// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_IsNoiseInRoom.h"

#include "BTTaskNode_GetNearestPatrolPoint.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorSchool/AI/AIHorrorEnemy.h"
#include "HorrorSchool/AI/HorrorEnemyAIController.h"
#include "HorrorSchool/Door/Door.h"
#include "HorrorSchool/Room/RoomVolume.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_IsNoiseInRoom::UBTT_IsNoiseInRoom()
{
	NodeName = "Is Noise in a Room";
	bCreateNodeInstance = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTT_IsNoiseInRoom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComponent = &OwnerComp;
	TargetDoor = nullptr;
	
	//Get the Controller and the pawn
	AIController = Cast<AHorrorEnemyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(AIController))
		return EBTNodeResult::Failed;
	
	AAIHorrorEnemy* HorrorEnemy = Cast<AAIHorrorEnemy>(AIController->GetPawn());
	if (!IsValid(HorrorEnemy))
		return EBTNodeResult::Failed;
	
	//Modify the Speed of the AI
	UCharacterMovementComponent* CharacterMovementComponent = HorrorEnemy->GetCharacterMovement();
	if (!IsValid(CharacterMovementComponent))
		return EBTNodeResult::Failed;
	
	CharacterMovementComponent->MaxWalkSpeed = MoveSpeed;
	
	//Stop the Sound from the AI
	HorrorEnemy->StopSound();
	
	FName RoomName = OwnerComp.GetBlackboardComponent()->GetValueAsName(TEXT("NoiseRoomName"));
	if (RoomName.IsNone())
		return EBTNodeResult::Failed;
	
	//If the AI is already in the room skip
	if (HorrorEnemy->GetCurrentRoomAI() == RoomName)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("NoiseRoomName"));
		return EBTNodeResult::Failed;
	}
	
	//Find the door for this room
	for (TObjectIterator<ADoor> It; It; ++It)
	{
		ADoor* Door = *It;
		if (!IsValid(Door))
			continue;
		
		if (Door->GetRoomVolume())
		{
			ARoomVolume* RoomVolume = Door->GetRoomVolume();
			if (RoomVolume->GetRoomName() == RoomName)
			{
				TargetDoor = Door;
				break;
			}
		}
	}
	
	if (!IsValid(TargetDoor))
		return EBTNodeResult::Failed;
	
	
	if (!IsValid(GetWorld()))
		return EBTNodeResult::Failed;
	
	if (UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
	{
		PFC->OnRequestFinished.RemoveAll(this);
		PFC->OnRequestFinished.AddUObject(this, &UBTT_IsNoiseInRoom::OnMoveToComplete);
	}
	
	//Project the Point on the NavMesh
	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
		GetWorld(),
		HorrorEnemy->GetActorLocation(),
		TargetDoor->GetActorLocation()
	);
	
	// If reachable move the AI to this point
	if (NavigationPath && NavigationPath->IsValid() && !NavigationPath->IsPartial())
	{
		AIController->MoveToActor(TargetDoor);
		return EBTNodeResult::InProgress;
	}
	
	FVector TargetLocation = TargetDoor->GetActorLocation();
	
	//Otherwise find the nearest point and then move the AI
	if (AIController->FindNearestPoint(TargetLocation))
	{
		AIController->MoveToLocation(TargetLocation);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTT_IsNoiseInRoom::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IsValid(AIController))
	{
		if (UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
		{
			PFC->OnRequestFinished.RemoveAll(this);
		}
		
		AIController->GetBlackboardComponent()->ClearValue(TEXT("NoiseRoomName"));
		AIController->StopMovement();
		AIController = nullptr;
	}
	
	if (IsValid(TargetDoor))
	{
		TargetDoor = nullptr;
	}
	
	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Aborted;
}

void UBTT_IsNoiseInRoom::OnMoveToComplete(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (IsValid(AIController))
	{
		if (UPathFollowingComponent* PFC = AIController->GetPathFollowingComponent())
		{
			PFC->OnRequestFinished.RemoveAll(this);
		}
		
		AIController->GetBlackboardComponent()->ClearValue(TEXT("NoiseRoomName"));
		AIController->StopMovement();
		AIController = nullptr;
	}
	
	if (!IsValid(BehaviorTreeComponent))
		return;	
	
	if (Result.Code == EPathFollowingResult::Success)
	{
		if (IsValid(TargetDoor))
		{
			TargetDoor = nullptr;
		}
		
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Failed);
	}
}