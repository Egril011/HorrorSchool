// Fill out your copyright notice in the Description page of Project Settings.


#include "RepairWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HorrorSchool/Notifier/ProgressNotifier.h"

void URepairWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ProgressBarWidget|| !TextWidget)
		return;
	
	ProgressBarWidget->SetFillColorAndOpacity(FLinearColor(FColor::FromHex("FF7A2AFF")));

	if (!ProgressNotifier)
		return;

	ProgressNotifier->OnProgressChanged.AddUniqueDynamic(this, &URepairWidget::SetProgressBarPercentage);
}

void URepairWidget::NativeDestruct()
{
	Super::NativeDestruct();

	//Remove all the references
	if (!ProgressNotifier)
		return;
	
	ProgressNotifier->OnProgressChanged.RemoveDynamic(this, &URepairWidget::SetProgressBarPercentage);
	ProgressNotifier = nullptr;
}

void URepairWidget::SetProgressBarPercentage(float Percent)
{
	if (!IsValid(ProgressBarWidget))
		return;
	
	ProgressBarWidget->SetPercent(Percent);

	//Change the progress bar to green when the repair is completed and update its text
	if (Percent >= 1.f)
	{
		FProgressBarStyle ProgressBarStyle = ProgressBarWidget->GetWidgetStyle();
		ProgressBarStyle.FillImage.TintColor = FLinearColor(FColor::FromHex("00FF00"));
		ProgressBarWidget->SetWidgetStyle(ProgressBarStyle);

		if (!IsValid(TextWidget))
			return;

		TextWidget->SetText(FText::FromString("Repaired - Reparer"));
	}
}