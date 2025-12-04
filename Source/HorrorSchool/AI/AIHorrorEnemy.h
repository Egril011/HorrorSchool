// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HorrorSchool/Room/RoomInterface.h"
#include "AIHorrorEnemy.generated.h"

class UCameraComponent;
class APatrolPath;

UCLASS()
class HORRORSCHOOL_API AAIHorrorEnemy : public ACharacter, public IRoomInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIHorrorEnemy();

	//Return the PatrolPath
	APatrolPath* GetPatrolPath() const {return PatrolPath;};
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Get in which room the AI is
	FName GetCurrentRoomAI() const {return CurrentRoomAI;};

protected:
	UPROPERTY(EditAnywhere, Category="PatrolPath")
	TObjectPtr<APatrolPath> PatrolPath;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	
	virtual void SetCurrentRoom_Implementation(FName Room) override;

private:
	FName CurrentRoomAI; 
};
