// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "HorrorSchool/Interact/Interfaces/InteractInterface.h"
#include "Door.generated.h"

class ARoomVolume;
class USoundEmitter;

UCLASS()
class HORRORSCHOOL_API ADoor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();
	
	// Return the link with the room volume
	ARoomVolume* GetRoomVolume() const {return DoorLinkRoomVolume;};

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="StaticMesh")
	TObjectPtr<UStaticMeshComponent> DoorMeshComponent;

	//Where the door has to go when the player opens it 
	UPROPERTY(EditAnywhere, Category = "Door|SceneComponent")
	TObjectPtr<USceneComponent> DoorSceneAnim;

	// The audio
	UPROPERTY(VisibleAnywhere, Category = "Door|Sound|Effect")
	TObjectPtr<UAudioComponent> DoorAudioComponent;

	UPROPERTY(VisibleAnywhere, Category = "Door|Sound")
	TObjectPtr<USoundEmitter> SoundEmitter;
	
	UPROPERTY(EditAnywhere, Category = "Door|SoundEffect")
	TObjectPtr<USoundBase> DoorSoundEffect;

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
	
	UPROPERTY(EditAnywhere, Category = "Door|AI", meta=(AllowPrivateAccess))
	TObjectPtr<ARoomVolume> DoorLinkRoomVolume;
};
