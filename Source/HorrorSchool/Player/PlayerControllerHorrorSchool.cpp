// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerHorrorSchool.h"

#include "Blueprint/UserWidget.h"
#include "HorrorSchool/Flashlight/Widgets/FlashlightBatteryWidget.h"
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
