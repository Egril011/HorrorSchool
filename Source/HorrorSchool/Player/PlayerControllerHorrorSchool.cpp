// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerHorrorSchool.h"

#include "Blueprint/UserWidget.h"
#include "HorrorSchool/Flashlight/Widgets/FlashlightBatteryWidget.h"

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
