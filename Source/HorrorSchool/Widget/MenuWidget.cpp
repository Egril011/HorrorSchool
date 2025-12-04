// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetIsFocusable(true);
	
	if (!IsValid(PlayerControllerRef))
		return;
	
	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	PlayerControllerRef->bShowMouseCursor = true;
	PlayerControllerRef->SetInputMode(InputModeUIOnly);
	
	if (IsValid(ButtonExitGame))
	{
		ButtonExitGame->OnClicked.RemoveAll(this);
		ButtonExitGame->OnClicked.AddUniqueDynamic(this, &UMenuWidget::QuitGame);
	}
	
	if (IsValid(ButtonRestart))
	{
		ButtonRestart->OnClicked.RemoveAll(this);
		ButtonRestart->OnClicked.AddUniqueDynamic(this, &UMenuWidget::RestartGame);
	}
}

void UMenuWidget::QuitGame()
{
	if (!IsValid(PlayerControllerRef))
		return;
	
	PlayerControllerRef->bShowMouseCursor = false;
	FInputModeGameOnly FInputModeGameOnly;
	PlayerControllerRef->SetInputMode(FInputModeGameOnly);
	PlayerControllerRef = nullptr;
	
	UGameplayStatics::OpenLevel(this, TEXT("MainMenu"));
}

void UMenuWidget::RestartGame()
{
	if (!IsValid(PlayerControllerRef))
		return;
	
	PlayerControllerRef->bShowMouseCursor = false;
	FInputModeGameOnly FInputModeGameOnly;
	PlayerControllerRef->SetInputMode(FInputModeGameOnly);
	PlayerControllerRef = nullptr;
	
	UGameplayStatics::OpenLevel(this, TEXT("HorrorSchool"));
}