// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryFlashlight.h"

#include "HorrorSchool/HorrorSchoolCharacter.h"
#include "HorrorSchool/Flashlight/Flashlight.h"
#include "HorrorSchool/Flashlight/Components/FlashlightBatteryComponent.h"

// Sets default values
ABatteryFlashlight::ABatteryFlashlight()
{
	BatteryStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BatteryStaticMeshComponent");
	RootComponent = BatteryStaticMeshComponent;
}

// Called when the game starts or when spawned
void ABatteryFlashlight::BeginPlay()
{
	Super::BeginPlay();
}

void ABatteryFlashlight::Interactable_Implementation(AActor* Interactor)
{
	//Cast to the player to get the flashlight 
	if (Interactor->IsA<AHorrorSchoolCharacter>())
	{
		AHorrorSchoolCharacter* Character = Cast<AHorrorSchoolCharacter>(Interactor);
		if (!IsValid(Character))
			return;
		
		//Get the battery component
		TScriptInterface<IUsableInterface> Flashlight = Character->GetEquippedItem();
		if (AFlashlight* PlayerFlashlight = Cast<AFlashlight>(Flashlight.GetObject()))
		{ 
			UFlashlightBatteryComponent* FlashlightBatteryComponent = 
				PlayerFlashlight->FindComponentByClass<UFlashlightBatteryComponent>();
			if (!IsValid(FlashlightBatteryComponent))
				return;
			
			FlashlightBatteryComponent->AddBatteryLife(BatteryIncreasement);
			Destroy();
		}
	}
}
