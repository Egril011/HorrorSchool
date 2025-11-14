// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"


#include "Components/FlashlightBatteryComponent.h"
#include "Components/SpotLightComponent.h"
#include "HorrorSchool/Items/Components/ItemAttachComponent.h"
#include "HorrorSchool/HorrorSchoolCharacter.h"
#include "HorrorSchool/Player/PlayerControllerHorrorSchool.h"

// Sets default values
AFlashlight::AFlashlight()
{
	FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMesh"));
	RootComponent = FlashlightMesh;

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLightComponent->SetupAttachment(FlashlightMesh);

	ItemAttachComponent = CreateDefaultSubobject<UItemAttachComponent>(TEXT("ItemAttachComponent"));
	BatteryComponent = CreateDefaultSubobject<UFlashlightBatteryComponent>(TEXT("BatteryComponent"));
}

void AFlashlight::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(SpotLightComponent))
		return;

	//hide the light
	SpotLightComponent->SetVisibility(false);
}

void AFlashlight::Interactable_Implementation(AActor* Interactor)
{
	//Attach the flashlight to the player
	if (!IsValid(ItemAttachComponent))
		return;

	//Remove its collider
	if (!IsValid(FlashlightMesh))
		return;

	FlashlightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (AHorrorSchoolCharacter* Character = Cast<AHorrorSchoolCharacter>(Interactor))
	{
		ItemAttachComponent->AttachItemToPlayer(Character);

		//make the player can use this item
		TScriptInterface<IUsableInterface> EquippedItem;
		EquippedItem.SetObject(this);
		EquippedItem.SetInterface(Cast<IUsableInterface>(this));
		Character->SetEquippedItem(EquippedItem);
	}

	//Show the UI
	if (APlayerControllerHorrorSchool* PC = Cast<APlayerControllerHorrorSchool>(GetWorld()->GetFirstPlayerController()))
	{
		if (!IsValid(BatteryComponent))
			return;
		
		PC->FlashlightNotifier = BatteryComponent->GetNotifier();
		PC->ShowFlashlightUI();
	}
}

void AFlashlight::Usable_Implementation()
{
	if (!IsValid(SpotLightComponent))
		return;

	if (SpotLightComponent->IsVisible())
		Flashlight_Off();
	else
		Flashlight_On();
}

void AFlashlight::Flashlight_On() const
{
	if (!IsValid(SpotLightComponent) || !IsValid(BatteryComponent))
		return;

	if (BatteryComponent->GetBatteryLife() > 0)
	{
		OnLightOn.Broadcast();
		SpotLightComponent->SetVisibility(true);
	}
}

void AFlashlight::Flashlight_Off() const
{
	if (!IsValid(SpotLightComponent))
		return;

	OnLightOff.Broadcast();
	SpotLightComponent->SetVisibility(false);
}