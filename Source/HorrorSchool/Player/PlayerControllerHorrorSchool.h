// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerHorrorSchool.generated.h"

class UNotificationWidget;
class UWinWidget;
class UMenuWidget;
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
	
	//UI Win
	void WinUI();
	
	//Show notification
	void ShowNotificationUI(FString Text,  float TimerCleanUp);
	
	//Clean up notification
	UFUNCTION()
	void CleanUpNotification();

	UPROPERTY()
	TObjectPtr<UFlashlightBatteryNotifier> FlashlightNotifier;

	UPROPERTY()
	TObjectPtr<UProgressNotifier> ProgressNotifier;
	
	UPROPERTY()
	TObjectPtr<UNotificationWidget> NotificationWidgetRef;
	
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
	TSubclassOf<UMenuWidget> JumpscareWidget;
	
	//Win
	UPROPERTY(EditAnywhere, Category="Widget", meta=(AllowPrivateAccess))
	TSubclassOf<UMenuWidget> WinWidget;
	
	//Notification
	UPROPERTY(EditAnywhere, Category="Widget", meta=(AllowPrivateAccess))
	TSubclassOf<UNotificationWidget> NotificationWidget;
};
