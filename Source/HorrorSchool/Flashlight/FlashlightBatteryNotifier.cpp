// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightBatteryNotifier.h"

void UFlashlightBatteryNotifier::BatteryPercentChangeBroadCast(float BatteryPercent) const
{
	OnBatteryPercentageChange.Broadcast(BatteryPercent);
}
