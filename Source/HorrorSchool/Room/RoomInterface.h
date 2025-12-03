// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RoomInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class URoomInterface : public UInterface
{
	GENERATED_BODY()
};

class IRoomInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCurrentRoom(FName Room);
};

