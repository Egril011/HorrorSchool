// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundEmitter.h"

#include "Perception/AISenseEvent.h"
#include "Perception/AISense_Hearing.h"

// Sets default values for this component's properties
USoundEmitter::USoundEmitter()
{
}

void USoundEmitter::EmitNoise(AActor* Actor)
{
	if (!IsValid(GetWorld()) || !IsValid(Actor))
		return;

	UAISense_Hearing::ReportNoiseEvent(
		GetWorld(),
		Actor->GetActorLocation(),
		1,
		Actor,
		3000.f);
}


