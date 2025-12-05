// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuScreenWidget.h"

#include "Components/Button.h"
#include "HorrorSchool/Player/PlayerControllerHorrorSchool.h"
#include "Kismet/GameplayStatics.h"

void UMenuScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	
	if (IsValid(PlayerControllerHorrorSchool))
	{
		PlayerControllerHorrorSchool->bShowMouseCursor = true;
		PlayerControllerHorrorSchool->SetIgnoreLookInput(true);
		PlayerControllerHorrorSchool->SetIgnoreMoveInput(true);
	}
	
	// Pause the game
	if (IsValid(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	
	if (IsValid(ButtonQuit))
	{
		ButtonQuit->OnClicked.RemoveAll(this);
		ButtonQuit->OnClicked.AddUniqueDynamic(this, &UMenuScreenWidget::QuitGame);
	}
	
	if (IsValid(ButtonResume))
	{
		ButtonResume->OnClicked.RemoveAll(this);
		ButtonResume->OnClicked.AddUniqueDynamic(this, &UMenuScreenWidget::ResumeGame);
	}
}

void UMenuScreenWidget::QuitGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}

void UMenuScreenWidget::ResumeGame()
{
	SetIsFocusable(false);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	if (IsValid(PlayerControllerHorrorSchool))
	{
		PlayerControllerHorrorSchool->CloseMenu();
		
		PlayerControllerHorrorSchool->bShowMouseCursor = false;
		PlayerControllerHorrorSchool->SetIgnoreLookInput(false);
		PlayerControllerHorrorSchool->SetIgnoreMoveInput(false);
	}
	
	PlayerControllerHorrorSchool = nullptr;
}
