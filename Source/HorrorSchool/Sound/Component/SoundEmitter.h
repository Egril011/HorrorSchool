// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoundEmitter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORSCHOOL_API USoundEmitter : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USoundEmitter();

	//Emit noise for the AI can hear the object
	void EmitNoise(AActor* Actor, float MaxRange);
};
