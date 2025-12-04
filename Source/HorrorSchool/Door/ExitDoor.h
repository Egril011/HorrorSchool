// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "ExitDoor.generated.h"

class AFuseBox;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API AExitDoor : public ADoor
{
	GENERATED_BODY()
	
protected:
	virtual void Interactable_Implementation(AActor* Interactor) override;
	
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AFuseBox>> FuseboxesArray;
	
	UPROPERTY(EditAnywhere)
	FString Text = "";
};
