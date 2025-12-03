// Fill out your copyright notice in the Description page of Project Settings.


#include "Locker.h"

#include "Components/TimelineComponent.h"
#include "HorrorSchool/HorrorSchoolCharacter.h"

// Sets default values
ALocker::ALocker()
{
	LockerBaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockerStaticMesh"));
	RootComponent = LockerBaseStaticMesh;
	
	LockerDoorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockerDoorStaticMesh"));
	LockerDoorStaticMesh->SetupAttachment(LockerBaseStaticMesh);
	
	LockerDoorLocationSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LockerDoorLocationSceneComponent"));
	LockerDoorLocationSceneComponent->SetupAttachment(LockerBaseStaticMesh);
	
	LockerTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("LockerTimeline"));
}

// Called when the game starts or when spawned
void ALocker::BeginPlay()
{
	Super::BeginPlay();
	
	// Save the Scene Component for the animation closes and opens
	if (!IsValid(LockerDoorStaticMesh) || !IsValid(LockerDoorLocationSceneComponent))
		return;
	
	LockerDoorStartLocation = LockerDoorStaticMesh->GetRelativeLocation();
	LockerDoorStartRotation = LockerDoorStaticMesh->GetRelativeRotation();
	LockerDoorEndLocation = LockerDoorLocationSceneComponent->GetRelativeLocation();
	LockerDoorEndRotation = LockerDoorLocationSceneComponent->GetRelativeRotation();
	
	//The timeline
	if (!IsValid(LockerTimeline) || !IsValid(LockerCurveFloat))
		return;
	
	FOnTimelineFloat OnTimelineFloat;
	OnTimelineFloat.BindUFunction(this, FName("OpenCloseTimelineFloat"));
	
	LockerTimeline->AddInterpFloat(LockerCurveFloat, OnTimelineFloat);
	LockerTimeline->SetLooping(false);
}

void ALocker::Interactable_Implementation(AActor* Interactor)
{
	//Open or Close the locker door
	if (Interactor->IsA<AHorrorSchoolCharacter>())
	{
		if (!bIsOpen)
		{
			Open();
		}
		else
		{
			Close();
		}
	}
}

void ALocker::Open()
{
	if (!IsValid(LockerTimeline))
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("TEst"));
	LockerTimeline->PlayFromStart();
}

void ALocker::Close()
{
	if (!IsValid(LockerTimeline))
		return;
	
	LockerTimeline->ReverseFromEnd();
}

void ALocker::OpenCloseTimelineFloat(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("TEst1"));
	FVector NewLocation = FMath::Lerp(LockerDoorStartLocation, LockerDoorEndLocation, Value);
	FRotator NewRotation = FMath::Lerp(LockerDoorStartRotation, LockerDoorEndRotation, Value);
	
	if (!IsValid(LockerDoorStaticMesh))
		return;
	
	LockerDoorStaticMesh->SetRelativeLocation(NewLocation);
	LockerDoorStaticMesh->SetRelativeRotation(NewRotation);
	
	if (Value >= 1.f)
	{
		bIsOpen = true;
	}
	else if (Value <= 0.f)
	{
		bIsOpen = false;
	}
}

