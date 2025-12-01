// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorrorSchool/Interact/Interfaces/InteractInterface.h"
#include "HorrorSchool/Items/Interfaces/UsableInterface.h"
#include "Flashlight.generated.h"

class AHorrorSchoolCharacter;
class UFlashlightBatteryComponent;
class USpotLightComponent;
class UPointLightComponent;
class UItemAttachComponent;


UCLASS()
class HORRORSCHOOL_API AFlashlight : public AActor, public IInteractInterface, public IUsableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlashlight();

	//Delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLightOn);
	FOnLightOn OnLightOn;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLightOff);
	FOnLightOff OnLightOff;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="StaticMesh")
	TObjectPtr<UStaticMeshComponent> FlashlightMesh;

	UPROPERTY(VisibleAnywhere, Category = "Flashlight|Component")
	TObjectPtr<UItemAttachComponent> ItemAttachComponent;

	UPROPERTY(VisibleAnywhere, Category = "Flashlight|Component")
	TObjectPtr<UFlashlightBatteryComponent> BatteryComponent;

private:
	virtual void Interactable_Implementation(AActor* Interactor) override;
	virtual void Usable_Implementation() override;

	/*Open and close the flashlight*/
	void Flashlight_On() const;
	void Flashlight_Off() const;
	
	UPROPERTY()
	USpotLightComponent* PlayerSpotLight = nullptr;
};
