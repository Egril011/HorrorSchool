// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomVolume.generated.h"

class UBoxComponent;

UCLASS()
class HORRORSCHOOL_API ARoomVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomVolume();
	
	static ARoomVolume* WhichRoomNoiseIs(const FVector& NoiseLocation);
	
	// Return the room name
	FName GetRoomName() const {return RoomName;};
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> RoomZone;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
private:
	UPROPERTY(EditAnywhere, Category="Room", meta=(AllowPrivateAcess))
	FName RoomName;
};
