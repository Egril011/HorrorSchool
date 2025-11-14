// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlashlightBatteryNotifier.generated.h"

/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UFlashlightBatteryNotifier : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBatteryPercentageChange, float, BatteryLifeValue);
	FOnBatteryPercentageChange OnBatteryPercentageChange;

	void BatteryPercentChangeBroadCast(float BatteryPercent) const;
};
