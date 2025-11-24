// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class APatrolPoint;

UCLASS()
class HORRORSCHOOL_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPoint")
	TArray<APatrolPoint*> PatrolPointPath;
};
