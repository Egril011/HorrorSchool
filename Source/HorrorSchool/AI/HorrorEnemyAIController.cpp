// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorEnemyAIController.h"

#include "AIHorrorEnemy.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HorrorSchool/HorrorSchoolCharacter.h"
#include "HorrorSchool/Room/RoomVolume.h"
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
			UE_LOG(LogTemp, Warning, TEXT("HearingLocation: %s"), *Stimulus.StimulusLocation.ToString())
			BlackboardComponent->SetValueAsBool(TEXT("BHearing"), true);
			
			//Detect which room the noise comes from
			const ARoomVolume* NoiseRoom = ARoomVolume::WhichRoomNoiseIs(Stimulus.StimulusLocation);
			if (IsValid(NoiseRoom))
			{
				GetBlackboardComponent()->SetValueAsName(TEXT("NoiseRoomName"), 
					NoiseRoom->GetRoomName());
			}
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

bool AHorrorEnemyAIController::FindNearestPoint(FVector& ObjectLocation)
{
	if (!IsValid(GetWorld()))
		return false;
	  
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!IsValid(NavSys))
		return false;

	FNavLocation NavLocation;
	FVector SearchLocation = ObjectLocation;

	DrawDebugSphere(
		GetWorld(),
		SearchLocation,
		10.f,
		12,
		FColor::Red,
		false,
		50.f,
		0,
		1.f
	);
	
	float Step = StartingStep;
	
	//Searching the nearest point around the DotLocation 
	while (Step < MaxStep)
	{
		FVector Extend = FVector(Step, Step, Step);
		if (NavSys->ProjectPointToNavigation(SearchLocation, NavLocation, Extend))
		{
			ObjectLocation = NavLocation.Location;
			DrawDebugSphere(
				GetWorld(),
				ObjectLocation,
				10.f,
				12,
				FColor::Blue,
				false,
				50.f,
				0,
				1.f
	);
			return true;
		}

		// If not found increase by 2 
		Step *= 2;
	}
	
	return false;
}