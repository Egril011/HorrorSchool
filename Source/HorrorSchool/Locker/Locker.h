// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorrorSchool/Interact/Interfaces/InteractInterface.h"
#include "Locker.generated.h"

class UTimelineComponent;

UCLASS()
class HORRORSCHOOL_API ALocker : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALocker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> LockerBaseStaticMesh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> LockerDoorStaticMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> LockerDoorLocationSceneComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Locker|Timeline")
	TObjectPtr<UTimelineComponent> LockerTimeline;
	
	virtual void Interactable_Implementation(AActor* Interactor) override;
	
private:
	UPROPERTY(EditAnywhere, Category="Locker|Timerline")
	TObjectPtr<UCurveFloat> LockerCurveFloat;
	
	bool bIsOpen = false;
	FVector LockerDoorStartLocation;
	FRotator LockerDoorStartRotation;
	FVector LockerDoorEndLocation;
	FRotator LockerDoorEndRotation;
	
	void Open();
	void Close();
	
	UFUNCTION()
	void OpenCloseTimelineFloat(float Value);
};