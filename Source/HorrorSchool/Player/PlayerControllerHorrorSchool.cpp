// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerHorrorSchool.h"

#include "Blueprint/UserWidget.h"
#include "HorrorSchool/Flashlight/Widgets/FlashlightBatteryWidget.h"
#include "HorrorSchool/Widget/MenuWidget.h"
#include "HorrorSchool/Widget/NotificationWidget.h"
#include "HorrorSchool/Widget/RepairWidget.h"

void APlayerControllerHorrorSchool::ShowFlashlightUI()
{
	if (!FlashlightNotifier)
		return;

	UFlashlightBatteryWidget* BatteryWidget = CreateWidget<UFlashlightBatteryWidget>(this,
		FlashlightWidgetClass);

	if (!IsValid(BatteryWidget))
		return;
	
	BatteryWidget->FlashlightBatteryNotifier = FlashlightNotifier;
	BatteryWidget->AddToViewport();
}

void APlayerControllerHorrorSchool::ShowFuseBoxUI()
{
	if (!ProgressNotifier)
		return;
	
	RepairWidget = CreateWidget<URepairWidget>(this, RepairWidgetClass);
	if (!IsValid(RepairWidget))
		return;

	RepairWidget->ProgressNotifier = ProgressNotifier;
	RepairWidget->AddToViewport();
}

void APlayerControllerHorrorSchool::CloseFuseBoxUI()
{
	if (!IsValid(RepairWidget))
		return;

	RepairWidget->RemoveFromParent();
	RepairWidget = nullptr;
	ProgressNotifier = nullptr;
}

void APlayerControllerHorrorSchool::JumpscareUI()
{
	if (!IsValid(JumpscareWidget))
		return;
	
	UMenuWidget* Jumpscare = CreateWidget<UMenuWidget>(this, JumpscareWidget);
	if (!IsValid(Jumpscare))
		return;
	
	Jumpscare->SetPlayerController(this);
	Jumpscare->AddToViewport();
}

void APlayerControllerHorrorSchool::WinUI()
{
	if (!IsValid(WinWidget))
		return;
	
	UMenuWidget* WinUI = CreateWidget<UMenuWidget>(this, WinWidget);
	if (!IsValid(WinUI))
		return;
	
	WinUI->SetPlayerController(this);
	WinUI->AddToViewport();
}

void APlayerControllerHorrorSchool::ShowNotificationUI(FString Text, float TimerCleanUp)
{
	if (!IsValid(NotificationWidget))
		return;
	
	Text = Text.Replace(TEXT("\\n"), TEXT("\n"));
	
	UNotificationWidget* NotificationUI = CreateWidget<UNotificationWidget>(this, NotificationWidget);
	if (!IsValid(NotificationUI))
		return;
	
	NotificationWidgetRef = NotificationUI;
	NotificationUI->SetText(Text);
	NotificationUI->AddToViewport();
	
	//Clean up the notification widget after X time
	if (!IsValid(GetWorld()))
		return;
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, 
		this,
		&APlayerControllerHorrorSchool::CleanUpNotification,
		TimerCleanUp,
		false);
}

void APlayerControllerHorrorSchool::CleanUpNotification()
{
	if (!IsValid(NotificationWidgetRef))
		return;
	
	NotificationWidgetRef->RemoveFromParent();
}