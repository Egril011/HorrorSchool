// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHorrorEnemy.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AAIHorrorEnemy::AAIHorrorEnemy()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

// Called to bind functionality to input
void AAIHorrorEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAIHorrorEnemy::SetCurrentRoom_Implementation(FName Room)
{
	CurrentRoomAI = Room;
} 