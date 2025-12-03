// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorrorSchool/Interact/Interfaces/InteractInterface.h"
#include "BatteryFlashlight.generated.h"

class UFlashlightBatteryComponent;

UCLASS()
class HORRORSCHOOL_API ABatteryFlashlight : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABatteryFlashlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BatteryStaticMeshComponent;
	
	virtual void Interactable_Implementation(AActor* Interactor) override;
	
	UPROPERTY(EditAnywhere)
	float BatteryIncreasement = 30.f;
};
