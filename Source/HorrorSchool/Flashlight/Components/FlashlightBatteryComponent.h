// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlashlightBatteryComponent.generated.h"

class UFlashlightBatteryNotifier;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORSCHOOL_API UFlashlightBatteryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlashlightBatteryComponent();

	//Get the battery life
	float GetBatteryLife() const { return BatteryLife; }
	
	//Set the battery life
	void AddBatteryLife(float IncreasementBatteryLife);
	
	//Get the FlashlightNotifier
	TObjectPtr<UFlashlightBatteryNotifier> GetNotifier() const { return  FlashlightBatteryNotifier; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Battery")
	TObjectPtr<UFlashlightBatteryNotifier> FlashlightBatteryNotifier;

private:
	float BatteryLife = 100;
	FTimerHandle FBatteryTimer;

	UPROPERTY(EditAnywhere, Category = "Battery|Variables", meta = (AllowPrivateAccess = "true"))
	float BatteryDecreasingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Battery|Variables", meta = (AllowPrivateAccess = "true"))
	float BatteryDrainAmount = 1.f;

	// Turn off the flashlight when the battery is dead
	void BatteryDead();

	// Decrease the battery life while the flashlight is turned on
	void DecreaseBatteryLife();

	// Stop the decrement of the battery life while the flashlight is turned off
	UFUNCTION()
	void StopDecreasingBatteryLife(); 

	// The timer for decreasing the battery life
	UFUNCTION()
	void BatteryTimer();
};
