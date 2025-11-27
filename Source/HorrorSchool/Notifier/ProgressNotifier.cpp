// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressNotifier.h"

void UProgressNotifier::ProgressChangeBroadCast(float Percentage)
{
	OnProgressChanged.Broadcast(Percentage);
}
