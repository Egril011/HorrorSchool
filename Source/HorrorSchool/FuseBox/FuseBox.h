// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorrorSchool/Interact/Interfaces/InteractInterface.h"
#include "FuseBox.generated.h"

class USoundEmitter;
class UBoxComponent;
class URepairWidget;
class UProgressNotifier;
class AHorrorSchoolCharacter;
class UTimelineComponent;

UCLASS()
class HORRORSCHOOL_API AFuseBox : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor 's properties
	AFuseBox();

protected:
	UPROPERTY(EditAnywhere, Category = "FuseBox")
	TObjectPtr<USkeletalMeshComponent> FuseBoxMeshComponent;
	
	UPROPERTY(EditAnywhere, Category = "FuseBox")
	TObjectPtr<USceneComponent> CameraLocation;

	UPROPERTY(EditAnywhere, Category = "FuseBox|Collider")
	TObjectPtr<UBoxComponent> FuseBoxCollider;

	virtual void BeginPlay() override;
	virtual void Interactable_Implementation(AActor* Interactor) override;
	virtual void InteractableHold_Implementation(AActor* Interactor, float HoldTime) override;
	virtual void InteractableRelease_Implementation(AActor* Interactor) override;

private:
	/*Animation open and close*/
	UPROPERTY(EditAnywhere, Category = "FuseBox|Animation", meta=(AllowPrivateAccess))
	TObjectPtr<UAnimationAsset> FuseAnimeOpen;

	UPROPERTY(EditAnywhere, Category = "FuseBox|Animation", meta=(AllowPrivateAccess))
	TObjectPtr<UAnimationAsset> FuseAnimeClose;

	/*Timeline*/
	UPROPERTY(VisibleAnywhere, Category = "FuseBox|TimeLine")
	TObjectPtr<UTimelineComponent> FuseBoxTimelineComponent;
	
	UPROPERTY(EditAnywhere, Category = "FuseBox|TimeLine", meta=(AllowPrivateAccess))
	TObjectPtr<UCurveFloat> FuseBoxCurve;
	
	/*Audio*/
	UPROPERTY(VisibleAnywhere, Category = "FuseBox|Audio")
	TObjectPtr<UAudioComponent> FuseBoxAudioComponent;
	
	UPROPERTY(EditAnywhere, Category = "FuseBox|Audio")
	TObjectPtr<USoundBase> FuseSoundBase;
	
	//AI
	UPROPERTY(VisibleAnywhere, Category = "FuseBox|AI")
	TObjectPtr<USoundEmitter> FuseboxSoundEmitter; 
	
	UPROPERTY(EditAnywhere, Category = "FuseBox|AI")
	float TimerSpeed = 3.f;

	/*Variables*/
	UPROPERTY()
	TObjectPtr<UProgressNotifier> ProgressNotifier;

	UPROPERTY()
	TObjectPtr<AHorrorSchoolCharacter> Player;
	FTransform PlayerCapsule;
	FTransform PlayerCamera;

	bool bIsRepaired = false;
	bool bIsInteracting = false;
	float CurrentHoldTime = 0.f;
	float FuseBoxPercentageRepaired = 0.f;

	UPROPERTY(EditAnywhere, Category = "FuseBox|Variable")
	float RequiredRepairTime = 3.0f;
	
	UFUNCTION()
	void TickCameraMoves(float Value);

	//When the player opens the fuse box
	void OpenFuseBox();

	//When the player closes the fuse box
	void CloseFuseBox();
	
	//Alert the AI
	FTimerHandle FuseTimerHandle;
	
	// X sec notify the AI with a timer
	UFUNCTION()
	void NotifyAI();
	
	void ClearNotifyAI();
};
