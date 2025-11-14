// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerHorrorSchool.generated.h"

class UFlashlightBatteryNotifier;
class UFlashlightBatteryWidget;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API APlayerControllerHorrorSchool : public APlayerController
{
	GENERATED_BODY()
public:
	//Create the flashlight widget class
	void ShowFlashlightUI();

	UPROPERTY()
	TObjectPtr<UFlashlightBatteryNotifier> FlashlightNotifier;

private:
	/*Flashlight*/
	UPROPERTY(EditAnywhere, Category="Widget", meta=(AllowPrivateAccess))
	TSubclassOf<UFlashlightBatteryWidget> FlashlightWidgetClass;
};
