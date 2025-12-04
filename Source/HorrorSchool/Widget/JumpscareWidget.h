// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpscareWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UJumpscareWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPlayerController(APlayerController* PlayerController) { PlayerControllerRef = PlayerController; }
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ButtonRestart;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ButtonExitGame;
	
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerControllerRef;
	
	//Return to the Main Menu
	UFUNCTION()
	void QuitGame();
	
	//Restart the Level
	UFUNCTION()
	void RestartGame();
};
