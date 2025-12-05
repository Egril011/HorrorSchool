// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"

#include "Components/WidgetSwitcher.h"

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	WidgetSwitcher->SetActiveWidget(NormalCrosshairWidget);
}

void UInteractWidget::UpdateInteractableWidget(const bool bIsLooking)
{
	if (!IsValid(WidgetSwitcher))
		return;
	
	if (bIsLooking && InteractableCrosshairWidget)
		WidgetSwitcher->SetActiveWidget(InteractableCrosshairWidget);
	else
		WidgetSwitcher->SetActiveWidget(NormalCrosshairWidget);
}
