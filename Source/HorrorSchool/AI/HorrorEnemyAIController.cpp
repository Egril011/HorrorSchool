// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorEnemyAIController.h"

#include "AIHorrorEnemy.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HorrorSchool/HorrorSchoolCharacter.h"
#include "PatrolPoint/PatrolPath.h"
#include "PatrolPoint/PatrolPoint.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

AHorrorEnemyAIController::AHorrorEnemyAIController()
{
	/** Perception **/
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	if (!IsValid(AIPerceptionComponent))
		return;

	SetPerceptionComponent(*AIPerceptionComponent);

	/**Sight**/
	AISenseSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseSight"));
	if (IsValid(AISenseSight))
	{
		AIPerceptionComponent->ConfigureSense(*AISenseSight);
		AIPerceptionComponent->SetDominantSense(AISenseSight->GetSenseImplementation());
		AISenseSight->DetectionByAffiliation.bDetectEnemies = true;
		AISenseSight->DetectionByAffiliation.bDetectFriendlies = true;
		AISenseSight->DetectionByAffiliation.bDetectNeutrals = true;
	}
	
	/*Hearting*/
	AISenseHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AISenseHearing"));
	if (IsValid(AISenseHearing))
	{
		AIPerceptionComponent->ConfigureSense(*AISenseHearing);
		AISenseHearing->DetectionByAffiliation.bDetectEnemies = true;
		AISenseHearing->DetectionByAffiliation.bDetectFriendlies = true;
		AISenseHearing->DetectionByAffiliation.bDetectNeutrals = true;
	}

	/**Blackboard**/
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}
  
void AHorrorEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	//save the patrol in the controller
	if (AAIHorrorEnemy* HorrorEnemy = Cast<AAIHorrorEnemy>(GetPawn()))
	{
		APatrolPath* PatrolPath = HorrorEnemy->GetPatrolPath();
		if (!IsValid(PatrolPath))
			return;

		for (APatrolPoint* PatrolPoint : PatrolPath->PatrolPointPath)
		{
			PatrolPointsAI.Add(PatrolPoint);
		}
	}
}

void AHorrorEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!IsValid(AIPerceptionComponent))
		return;
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,
		&AHorrorEnemyAIController::OnTargetPerceptionUpdated);

	if (IsValid(BehaviourTree) && IsValid(BlackboardComponent))
	{
		UseBlackboard(BehaviourTree->GetBlackboardAsset(), BlackboardComponent);
		RunBehaviorTree(BehaviourTree);
	}
}

/*void AHorrorEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		// Controller is not possessing anything right now
		return;
	}

	if (!GetWorld())
	{
		return;
	}


	FVector Location = GetPawn()->GetActorLocation();
	FVector Forward = GetPawn()->GetActorForwardVector();
	
	/*Debug the Slight*/
	/*if (AIPerceptionComponent && GetPawn())
	{
		DrawDebugSphere(GetWorld(), Location, AISenseSight->SightRadius, 32, FColor::Green);
		DrawDebugSphere(GetWorld(), Location, AISenseSight->LoseSightRadius, 32, FColor::Red);
		
		DrawDebugCone(
			GetWorld(),
			Location,
			Forward,
			AISenseSight->SightRadius,
			FMath::DegreesToRadians(AISenseSight->PeripheralVisionAngleDegrees),
			FMath::DegreesToRadians(AISenseSight->PeripheralVisionAngleDegrees),
			32,
			FColor::Yellow
		);
	}

	//Debug Hearing
	if (AISenseHearing && GetPawn())
	{
		DrawDebugSphere(GetWorld(), Location, AISenseHearing->HearingRange, 32, FColor::Blue);
	}
}*/

void AHorrorEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!IsValid(Actor) || Actor == GetPawn())
		return;

	if (Stimulus.WasSuccessfullySensed())
	{
		if (!IsValid(BlackboardComponent))
			return;
		
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			if (Actor->IsA<AHorrorSchoolCharacter>())
			{
				if (BlackboardComponent->GetValueAsObject(TEXT("Player")) != Actor)
				{
					BlackboardComponent->SetValueAsObject(TEXT("Player"), Actor);
				}
			}
		}
		else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			BlackboardComponent->SetValueAsVector(TEXT("HearingLocation"), Stimulus.StimulusLocation);
			BlackboardComponent->SetValueAsBool(TEXT("BHearing"), true);
		}
	}
	else
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			BlackboardComponent->ClearValue(TEXT("Player"));
		}
		else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			BlackboardComponent->SetValueAsBool(TEXT("BHearing"), false);
		}
	}
}