// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateInteractableWidget(const bool bIsLooking);
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUserWidget> NormalCrosshairWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUserWidget> InteractableCrosshairWidget;
};
