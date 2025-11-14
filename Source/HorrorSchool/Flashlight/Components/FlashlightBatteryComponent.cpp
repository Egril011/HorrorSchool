// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightBatteryComponent.h"
#include "Components/LightComponent.h"
#include "HorrorSchool/Flashlight/Flashlight.h"
#include "HorrorSchool/Flashlight/FlashlightBatteryNotifier.h"

// Sets default values for this component's properties
UFlashlightBatteryComponent::UFlashlightBatteryComponent()
{
}

// Called when the game starts
void UFlashlightBatteryComponent::BeginPlay() 
{
	Super::BeginPlay();

	//Use the delegates to detect when the player turns the flashlight on or off
	AFlashlight* Flashlight = Cast<AFlashlight>(GetOwner());
	if (!IsValid(Flashlight))
		return;
   
	Flashlight->OnLightOn.AddUniqueDynamic(this, &UFlashlightBatteryComponent::BatteryTimer);
	Flashlight->OnLightOff.AddUniqueDynamic(this, &UFlashlightBatteryComponent::StopDecreasingBatteryLife);

	//Listener for the battery life
	FlashlightBatteryNotifier = NewObject<UFlashlightBatteryNotifier>(this);
}

void UFlashlightBatteryComponent::BatteryDead()
{
	if (IsValid(GetWorld()))
		GetWorld()->GetTimerManager().ClearTimer(FBatteryTimer);
	
	//Cast to the Flashlight class to turn off the light associated
	if (AFlashlight* Flashlight = Cast<AFlashlight>(GetOwner()))
	{
		ULightComponent* LightComponent = Flashlight->FindComponentByClass<ULightComponent>();
		if (!IsValid(LightComponent))
			return;

		LightComponent->SetVisibility(false);
	}
}

void UFlashlightBatteryComponent::DecreaseBatteryLife()
{
	if (!IsValid(FlashlightBatteryNotifier))
		return;
	
	BatteryLife -= BatteryDrainAmount;
	FlashlightBatteryNotifier->BatteryPercentChangeBroadCast(BatteryLife);
	if (BatteryLife <= 0)
		BatteryDead();
}

void UFlashlightBatteryComponent::StopDecreasingBatteryLife()
{
	GetWorld()->GetTimerManager().ClearTimer(FBatteryTimer);
}

void UFlashlightBatteryComponent::BatteryTimer()
{
	if (!IsValid(GetWorld()))
		return;

	// Start the timer to decrease the battery life
	GetWorld()->GetTimerManager().ClearTimer(FBatteryTimer);
	GetWorld()->GetTimerManager().SetTimer(
		FBatteryTimer,
		this,
		&UFlashlightBatteryComponent::DecreaseBatteryLife,
		BatteryDecreasingSpeed,
		true
		);
}
  
