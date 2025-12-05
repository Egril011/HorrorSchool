// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerHorrorSchool.generated.h"

class UInteractWidget;
class UMenuScreenWidget;
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
	
	//Open the menu
	void OpenMenu();
	
	//Close the menu
	void CloseMenu();
	
	//Interact Menu
	void Interact(bool bInteract);
	
	void CleanUpInteract();

	UPROPERTY()
	TObjectPtr<UFlashlightBatteryNotifier> FlashlightNotifier;

	UPROPERTY()
	TObjectPtr<UProgressNotifier> ProgressNotifier;
	
private:
	UPROPERTY()
	TObjectPtr<UNotificationWidget> NotificationWidgetRef;

	UPROPERTY()
	TObjectPtr<UMenuScreenWidget> MenuWidgetRef;
	
	UPROPERTY()
	TObjectPtr<UInteractWidget> InteractWidgetRef;
	
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
	
	//Menu
	UPROPERTY(EditAnywhere, Category="Widget", meta=(AllowPrivateAccess))
	TSubclassOf<UMenuScreenWidget> MenuWidget;
	
	//Interact
	UPROPERTY(EditAnywhere, Category="Widget", meta=(AllowPrivateAccess))
	TSubclassOf<UInteractWidget> InteractWidget;
	
	bool bMenuOpen = false;
	bool bInteractWidget = false;
	bool bOnaObject = false;
};
