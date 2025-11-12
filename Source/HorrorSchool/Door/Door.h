// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "HorrorSchool/Interact/InteractInterface.h"
#include "Door.generated.h"

UCLASS()
class HORRORSCHOOL_API ADoor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="StaticMesh")
	TObjectPtr<UStaticMeshComponent> DoorMeshComponent;

	//Where the door has to go when the player opens it 
	UPROPERTY(EditAnywhere, Category = "Door|SceneComponent")
	TObjectPtr<USceneComponent> DoorSceneAnim;

private:
	virtual void Interactable_Implementation(AActor* Actor) override;

	/*Open and close the door*/
	void OpenDoor() const;
	void CloseDoor() const;
	UFUNCTION()
	void TickOpenCloseDoor(float Value);

	/*For the animation*/
	FVector DoorLocation;
	FVector EndDoorLocation;
	bool bDoorOpen;

	UPROPERTY(EditAnywhere, Category = "Door|Timeline")
	TObjectPtr<UTimelineComponent> DoorTimelineComponent;

	UPROPERTY(EditAnywhere, Category = "Door|Timeline")
	TObjectPtr<UCurveFloat> DoorCurve;
};
