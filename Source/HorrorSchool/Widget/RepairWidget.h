// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RepairWidget.generated.h"

class UProgressNotifier;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API URepairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UProgressNotifier> ProgressNotifier;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> ProgressBarWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextWidget;

private:
	UFUNCTION()
	void SetProgressBarPercentage(float Percent);
};
