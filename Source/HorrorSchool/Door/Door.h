// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(EditAnywhere, Category="StaticMesh")
	TObjectPtr<UStaticMeshComponent> DoorMeshComponent;

private:
	void Interactable_Implementation(AActor* Actor) override;
};
