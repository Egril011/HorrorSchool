// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerHorrorSchool.generated.h"

class UJumpscareWidget;
class URepairWidget;
class UProgressNotifier;
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

	//Create the fuseBox widget class
	void ShowFuseBoxUI();

	//Close the fuseBox widget class
	void CloseFuseBoxUI();
	
	//UI for the JumpScare
	void JumpscareUI();

	UPROPERTY()
	TObjectPtr<UFlashlightBatteryNotifier> FlashlightNotifier;

	UPROPERTY()
	TObjectPtr<UProgressNotifier> ProgressNotifier;
	
private:
	/*Flashlight*/
	UPROPERTY(EditAnywhere, Category="Widget", meta=(AllowPrivateAccess))
	TSubclassOf<UFlashlightBatteryWidget> FlashlightWidgetClass;

	//FuseBox
	UPROPERTY(EditAnywhere, Category="Widget", meta=(AllowPrivateAccess))
	TSubclassOf<URepairWidget> RepairWidgetClass;
	
	UPROPERTY()
	TObjectPtr<URepairWidget> RepairWidget;
	
	//Jumpscare
	UPROPERTY(EditAnywhere, Category="Widget", meta=(AllowPrivateAccess))
	TSubclassOf<UJumpscareWidget> JumpscareWidget;
};
