// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightBatteryWidget.h"
#include "Components/ProgressBar.h"
#include "HorrorSchool/Flashlight/FlashlightBatteryNotifier.h"

void UFlashlightBatteryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(BatteryLifeBar) || !IsValid(FlashlightBatteryNotifier))
		return;   
	
	FlashlightBatteryNotifier->OnBatteryPercentageChange.AddUniqueDynamic(this,
		&UFlashlightBatteryWidget::SetBatteryLifeBarPercent);
	
	BatteryLifeBar->SetPercent(1);
}

void UFlashlightBatteryWidget::SetBatteryLifeBarPercent(float Percent)
{
	if (!IsValid(BatteryLifeBar))
		return;

	float const BatteryLifeBarPercent = Percent/100.f;
	BatteryLifeBar->SetPercent(BatteryLifeBarPercent);
}
