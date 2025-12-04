// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetText(FString NewText) { Text = NewText; };
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock;
	
	FString Text = "";
};
