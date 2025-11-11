// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

// Sets default values
ADoor::ADoor()
{
	DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshComponent"));
}

void ADoor::Interactable_Implementation(AActor* Actor)
{
	UE_LOG(LogTemp, Log, TEXT("Interactable_Implementation"));
}
