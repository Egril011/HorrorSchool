// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/AudioComponent.h"
#include "HorrorSchool/HorrorSchoolCharacter.h"
#include "HorrorSchool/AI/AIHorrorEnemy.h"
#include "HorrorSchool/Sound/Component/SoundEmitter.h"

// Sets default values
ADoor::ADoor()
{
	DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshComponent"));
	RootComponent = DoorMeshComponent;

	DoorSceneAnim = CreateDefaultSubobject<USceneComponent>(TEXT("DoorSceneAnim"));
	DoorSceneAnim->SetupAttachment(RootComponent);

	DoorTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimelineComponent"));

	DoorAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DoorAudioComponent"));
	DoorAudioComponent->SetupAttachment(DoorMeshComponent);
	DoorAudioComponent->bAutoActivate = false;

	SoundEmitter = CreateDefaultSubobject<USoundEmitter>(TEXT("SoundEmitter"));
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	DoorLocation = GetActorLocation();
	EndDoorLocation = DoorSceneAnim->GetComponentLocation();

	if(!IsValid(DoorTimelineComponent) || !IsValid(DoorCurve))
		return;
	
	//For the timeline (animation)
	FOnTimelineFloat OnTimelineFloat;
	OnTimelineFloat.BindUFunction(this, FName("TickOpenCloseDoor"));
	
	DoorTimelineComponent->AddInterpFloat(DoorCurve, OnTimelineFloat);
	DoorTimelineComponent->SetLooping(false);
}

void ADoor::Interactable_Implementation(AActor* Actor)
{
	if (!IsValid(DoorTimelineComponent))
		return;

	if (DoorTimelineComponent->IsPlaying() || DoorTimelineComponent->IsReversing())
		return;
	
	//Noise for the AI
	if (IsValid(SoundEmitter))
	{
		SoundEmitter->EmitNoise(this, 3000.f);
	}
	
	if (!bDoorOpen)
		OpenDoor();
	else
	{
		CloseDoor();
	}
}

void ADoor::OpenDoor() const
{
	if (!IsValid(DoorTimelineComponent))
		return;
	
	if (IsValid(DoorSoundEffect) && IsValid(DoorAudioComponent))
	{
		DoorAudioComponent->SetSound(DoorSoundEffect);
		DoorAudioComponent->Play();
	}
	
	DoorTimelineComponent->PlayFromStart();
}

void ADoor::CloseDoor() const
{
	if (!IsValid(DoorTimelineComponent))
		return;

	if (IsValid(DoorSoundEffect) && IsValid(DoorAudioComponent))
	{
		DoorAudioComponent->SetSound(DoorSoundEffect);
		DoorAudioComponent->Play();
	}
	
	DoorTimelineComponent->ReverseFromEnd();
}

void ADoor::TickOpenCloseDoor(float Value)
{
	FVector NewLocation = FMath::Lerp(DoorLocation, EndDoorLocation, Value);
	SetActorLocation(NewLocation);
	
	if (Value >= 1.f)
		bDoorOpen = true;
	
	else if (Value <= 0.f)
		bDoorOpen = false;
}