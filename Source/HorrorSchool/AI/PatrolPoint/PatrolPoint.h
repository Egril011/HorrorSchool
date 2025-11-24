// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPoint.generated.h"  

class USphereComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORSCHOOL_API APatrolPoint : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	APatrolPoint();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PatrolPoint")
	TObjectPtr<USceneComponent> PatrolPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPoint")
	TObjectPtr<USphereComponent> SphereComponent;
};
