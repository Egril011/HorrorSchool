// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FlashlightBatteryWidget.generated.h"

class UFlashlightBatteryNotifier;
class UFlashlightBatteryComponent;
class UProgressBar;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UFlashlightBatteryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UFlashlightBatteryNotifier> FlashlightBatteryNotifier;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> BatteryLifeBar;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetBatteryLifeBarPercent(float Percent);
};
