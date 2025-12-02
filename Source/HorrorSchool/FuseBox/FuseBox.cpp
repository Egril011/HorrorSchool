// Fill out your copyright notice in the Description page of Project Settings.


#include "FuseBox.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorSchool/HorrorSchoolCharacter.h"
#include "HorrorSchool/Notifier/ProgressNotifier.h"
#include "HorrorSchool/Player/PlayerControllerHorrorSchool.h"
#include "HorrorSchool/Sound/Component/SoundEmitter.h"

// Sets default values
AFuseBox::AFuseBox()
{
 	FuseBoxMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = FuseBoxMeshComponent;

	CameraLocation = CreateDefaultSubobject<USceneComponent>(TEXT("CameraLocation"));
	CameraLocation->SetupAttachment(FuseBoxMeshComponent);

	FuseBoxTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("FuseBoxTimeline"));

	FuseBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("FuseBoxCollider"));
	FuseBoxCollider->SetupAttachment(FuseBoxMeshComponent);
	
	FuseBoxAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FuseboxAudioComponent"));
	FuseBoxAudioComponent->SetupAttachment(FuseBoxMeshComponent);
	FuseBoxAudioComponent->bAutoActivate = false;
	
	FuseboxSoundEmitter = CreateDefaultSubobject<USoundEmitter>(TEXT("FuseboxSoundEmitter"));
}

void AFuseBox::BeginPlay()
{
	Super::BeginPlay();

	//Create the notifier for the Widget
	ProgressNotifier = NewObject<UProgressNotifier>(this);
	
	if (!IsValid(FuseBoxTimelineComponent) || !IsValid(FuseBoxCurve))
		return;

	//For the Timeline
	FOnTimelineFloat OnTimelineFloat;
	OnTimelineFloat.BindUFunction(this, FName("TickCameraMoves"));
	FuseBoxTimelineComponent->AddInterpFloat(FuseBoxCurve, OnTimelineFloat);
	FuseBoxTimelineComponent->SetLooping(false);

}

void AFuseBox::Interactable_Implementation(AActor* Interactor)
{
	if (bIsRepaired)
		return;

	Player = Cast<AHorrorSchoolCharacter>(Interactor);
	if (!IsValid(Player))
		return;
	
	if (!bIsInteracting)
	{
		OpenFuseBox();
	}
	else
	{
		CloseFuseBox();
	}
}

void AFuseBox::InteractableHold_Implementation(AActor* Interactor, const float HoldTime)
{
	if (!ProgressNotifier)
		return;

	if (!bIsInteracting)
		return;
	
	// Audio
	if (IsValid(FuseBoxAudioComponent) && IsValid(FuseSoundBase))
	{
		if (HoldTime <= 0)
		{
			FuseBoxAudioComponent->Stop();
		}
		else
		{
			if (FuseBoxAudioComponent->Sound != FuseSoundBase)
			{
				FuseBoxAudioComponent->SetSound(FuseSoundBase);
			}
			
			if (!FuseBoxAudioComponent->IsPlaying())
			{
				FuseBoxAudioComponent->Play();
				
				//Start the time to notify the AI
				if (!IsValid(GetWorld()))
					return;
				
				GetWorld()->GetTimerManager().ClearTimer(FuseTimerHandle);
				GetWorld()->GetTimerManager().SetTimer(
					FuseTimerHandle,
					this,
					&AFuseBox::NotifyAI,
					TimerSpeed,
					true,
					0
					);
			}
		}
	}
	
	//To increase the progress bar's percentage
	FuseBoxPercentageRepaired += HoldTime / RequiredRepairTime;
	FuseBoxPercentageRepaired = FMath::Clamp(FuseBoxPercentageRepaired, 0.f, 1.f);

	ProgressNotifier->ProgressChangeBroadCast(FuseBoxPercentageRepaired);

	//The repair is done
	if (FuseBoxPercentageRepaired >= 1.f)
	{
		bIsRepaired = true;
		
		if (!IsValid(GetWorld()))
			return;
		
		//Close the fuse box
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&AFuseBox::CloseFuseBox,
			1.f,
			false,
			-1
			);
	}
}

void AFuseBox::InteractableRelease_Implementation(AActor* Interactor)
{
	if (IsValid(FuseBoxAudioComponent) && FuseBoxAudioComponent->IsPlaying())
	{
		FuseBoxAudioComponent->Stop();
		ClearNotifyAI();
	}
}

void AFuseBox::TickCameraMoves(float Value)
{
	// Move the player's capsule and its camera to the camera location
	FTransform NewPlayerCapsuleTransform;
	NewPlayerCapsuleTransform.Blend(PlayerCapsule,CameraLocation->GetComponentTransform(),
		Value);

	FTransform NewPlayerCameraTransform;
	NewPlayerCameraTransform.Blend(PlayerCamera, CameraLocation->GetComponentTransform(),
		Value);
	
	Player->GetCapsuleComponent()->SetWorldTransform(NewPlayerCapsuleTransform);
	Player->GetFirstPersonCameraComponent()->SetWorldTransform(NewPlayerCameraTransform);
}

void AFuseBox::OpenFuseBox()
{
	bIsInteracting = true;

	//Play the open animation
	if (!IsValid(FuseAnimeOpen) || !IsValid(FuseBoxMeshComponent) || !IsValid(FuseBoxTimelineComponent))
		return;
		
	FuseBoxMeshComponent->PlayAnimation(FuseAnimeOpen, false);

	if (!IsValid(Player))
		return;
		
	Player->GetCharacterMovement()->DisableMovement();
	PlayerCapsule = Player->GetCapsuleComponent()->GetComponentTransform();
	PlayerCamera = Player->GetFirstPersonCameraComponent()->GetComponentTransform();

	FuseBoxTimelineComponent->PlayFromStart();

	//Show the fuse box UI
	APlayerControllerHorrorSchool* PlayerController = Cast<APlayerControllerHorrorSchool>(Player->GetController());
	if (!IsValid(PlayerController))
		return;

	if (!IsValid(ProgressNotifier))
		return;
	
	PlayerController->ProgressNotifier = ProgressNotifier;
	PlayerController->ShowFuseBoxUI();

	/* If the player previously interacted with the fuse box but didn't finish repairing it,
	 * then restore the progress bar to the last saved percentage*/
	ProgressNotifier->ProgressChangeBroadCast(FuseBoxPercentageRepaired);
}

void AFuseBox::CloseFuseBox()
{
	bIsInteracting = false;

	//Play the close animation
	if (!IsValid(FuseAnimeClose)|| !IsValid(FuseBoxMeshComponent) || !IsValid(FuseBoxTimelineComponent))
		return;

	FuseBoxMeshComponent->PlayAnimation(FuseAnimeClose, false);
	
	if (!IsValid(Player))
		return;
	
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	//Remove the Widget
	APlayerControllerHorrorSchool* PlayerControllerHorrorSchool = Cast<APlayerControllerHorrorSchool>(Player->GetController());
	if (!IsValid(PlayerControllerHorrorSchool))
		return;

	PlayerControllerHorrorSchool->CloseFuseBoxUI();
	FuseBoxTimelineComponent->ReverseFromEnd();
	
	//Stop the Audio
	if (IsValid(FuseBoxAudioComponent) && FuseBoxAudioComponent->IsPlaying())
	{
		FuseBoxAudioComponent->Stop();
		ClearNotifyAI();
	}
}

void AFuseBox::NotifyAI()
{
	if (!IsValid(FuseboxSoundEmitter))
		return;
	
	FuseboxSoundEmitter->EmitNoise(this, 4000.f);
}

void AFuseBox::ClearNotifyAI()
{
	if (!IsValid(GetWorld()))
		return;
	
	GetWorld()->GetTimerManager().ClearTimer(FuseTimerHandle);
}