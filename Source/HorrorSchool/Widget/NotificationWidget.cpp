 // Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationWidget.h"

#include "Components/TextBlock.h"

 void UNotificationWidget::NativeConstruct()
 {
	 Super::NativeConstruct();
 	
 	if (IsValid(TextBlock))
 	{
 		TextBlock->SetText(FText::FromString(Text));
 	}
 }

 void UNotificationWidget::NativeDestruct()
 {
	 Super::NativeDestruct();
 	
 	Text = "";
 	TextBlock->SetText(FText::FromString(Text));
 }