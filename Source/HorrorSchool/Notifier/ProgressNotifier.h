// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProgressNotifier.generated.h"

/**
 * 
 */
UCLASS()
class HORRORSCHOOL_API UProgressNotifier : public UObject
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProgressChanged, float, Value);
	FOnProgressChanged OnProgressChanged;

	void ProgressChangeBroadCast(float Percentage);
};
