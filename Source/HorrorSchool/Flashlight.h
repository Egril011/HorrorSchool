// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableInterface.h"
#include "GameFramework/Actor.h"
#include "Interact/InteractInterface.h"
#include "Flashlight.generated.h"

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

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="StaticMesh")
	TObjectPtr<UStaticMeshComponent> FlashlightMesh;

	UPROPERTY(EditAnywhere, Category="Flashlight|light")
	TObjectPtr<USpotLightComponent> SpotLightComponent;

	UPROPERTY(VisibleAnywhere, Category = "Flashlight|Component")
	TObjectPtr<UItemAttachComponent> ItemAttachComponent;

private:
	virtual void Interactable_Implementation(AActor* Interactor) override;
	virtual void Usable_Implementation() override;

	/*Open and close the flashlight*/
	void Flashlight_On() const;
	void Flashlight_Off() const;
};
