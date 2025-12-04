// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHorrorEnemy.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AAIHorrorEnemy::AAIHorrorEnemy()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->SetupAttachment(RootComponent);
}

void AAIHorrorEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AudioComponent) && IsValid(AISound))
	{
		AudioComponent->SetSound(AISound);
		AudioComponent->Play();
	}
}

// Called to bind functionality to input
void AAIHorrorEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAIHorrorEnemy::PlaySound()
{
	if (IsValid(AudioComponent))
	{
		if (AudioComponent->IsPlaying())
			return;
		
		AudioComponent->Play();
	}
}

void AAIHorrorEnemy::StopSound()
{
	if (IsValid(AudioComponent))
	{
		if (AudioComponent->IsPlaying())
		{
			AudioComponent->Stop();
		}
	}
}

void AAIHorrorEnemy::SetCurrentRoom_Implementation(FName Room)
{
	CurrentRoomAI = Room;
} 