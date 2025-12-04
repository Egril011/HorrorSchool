// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_JumpScare.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "HorrorSchool/HorrorSchoolCharacter.h"
#include "HorrorSchool/AI/AIHorrorEnemy.h"
#include "HorrorSchool/AI/HorrorEnemyAIController.h"
#include "HorrorSchool/Player/PlayerControllerHorrorSchool.h"

UBTT_JumpScare::UBTT_JumpScare()
{
	NodeName = "JumpScare";
	bCreateNodeInstance = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTT_JumpScare::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComponent = &OwnerComp;
	
	AIController = Cast<AHorrorEnemyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(AIController))
		return EBTNodeResult::Failed;
	
	AIController->StopMovement();
	
	// Get the Pawn
	AIHorrorEnemy = Cast<AAIHorrorEnemy>(AIController->GetPawn());
	if (!IsValid(AIController))
		return EBTNodeResult::Failed;
	
	// Get the Player
	Player = Cast<AHorrorSchoolCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
	if (!IsValid(Player))
		return EBTNodeResult::Failed;
	
	//Show the UI 
	if (!bShowWidget)
	{
		APlayerControllerHorrorSchool* PlayerControllerHorrorSchool = Cast<APlayerControllerHorrorSchool>(Player->GetController());
		if (!IsValid(PlayerControllerHorrorSchool))
			return EBTNodeResult::Failed;
		
		PlayerControllerHorrorSchool->JumpscareUI();
		bShowWidget = true;
	}
	
	//Shake the Camera
	MovePlayerCameraToPawnCamera();
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTT_JumpScare::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComponent = nullptr;
	AIController = nullptr;
	Player = nullptr;
	bShowWidget = false;
	
	FinishLatentAbort(OwnerComp);
	return EBTNodeResult::Aborted;
}

void UBTT_JumpScare::MovePlayerCameraToPawnCamera()
{
	if (!IsValid(AIHorrorEnemy) || !IsValid(Player))
		return;
	
	//Move the player camera to the pawn camera
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	if (!IsValid(PlayerController))
		return;
	
	PlayerController->SetViewTargetWithBlend(AIHorrorEnemy, 2.f, 
		EViewTargetBlendFunction::VTBlend_Cubic);
	
	// Shake the camera
	PlayerController->ClientStartCameraShake(CameraShake);
}