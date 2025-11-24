// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPoint.h"

#include "Components/SphereComponent.h"

// Sets default values for this component's properties
APatrolPoint::APatrolPoint()
{
	PatrolPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PatrolPoint"));
	RootComponent = PatrolPoint;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(PatrolPoint);
}

