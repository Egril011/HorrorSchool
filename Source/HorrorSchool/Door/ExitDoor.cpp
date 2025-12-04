// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor.h"

#include "HorrorSchool/FuseBox/FuseBox.h"
#include "HorrorSchool/HorrorSchoolCharacter.h"
#include "HorrorSchool/Player/PlayerControllerHorrorSchool.h"

void AExitDoor::Interactable_Implementation(AActor* Interactor)
{
	bool AllRepaired = true;
	
	if (Interactor->IsA<AHorrorSchoolCharacter>())
	{
		APlayerControllerHorrorSchool* PlayerController = Cast<APlayerControllerHorrorSchool>(
				Interactor->GetInstigatorController());
		
		//Check if all the Fusebox are repaired
		for (AFuseBox* Box : FuseboxesArray)
		{
			if (!Box || !Box->GetIsRepaired())
			{
				AllRepaired = false;
				break;
			}
		}
	
		// All repaired show the UI
		if (AllRepaired)
		{
			if (!IsValid(PlayerController))
				return;
			
			PlayerController->WinUI();
		}
		else
		{
			PlayerController->ShowNotificationUI(Text, 10.f);
		}
	}
}