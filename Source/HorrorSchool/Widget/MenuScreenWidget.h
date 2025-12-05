// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuScreenWidget.generated.h"

class APlayerControllerHorrorSchool;
class UButton;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UMenuScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPlayerControllerRef(APlayerControllerHorrorSchool* PlayerController) { PlayerControllerHorrorSchool = PlayerController; }
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ButtonQuit;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ButtonResume;
	
private:
	UPROPERTY()
	TObjectPtr<APlayerControllerHorrorSchool> PlayerControllerHorrorSchool;
	
	//Return to the Main Menu
	UFUNCTION()
	void QuitGame();
	
	//Resume the game
	UFUNCTION()
	void ResumeGame();
};
