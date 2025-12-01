// Copyright Epic Games, Inc. All Rights Reserved.

#include "HorrorSchoolCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/SpotLightComponent.h"
#include "Engine/LocalPlayer.h"
#include "Interact/Components/InteractComponent.h"
#include "Items/Interfaces/UsableInterface.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AHorrorSchoolCharacter

AHorrorSchoolCharacter::AHorrorSchoolCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	//Components
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	ItemAttachment = CreateDefaultSubobject<USceneComponent>(TEXT("ItemAttachment"));
	ItemAttachment->SetupAttachment(FirstPersonCameraComponent);
	ItemAttachment->SetRelativeLocation(FVector(30.f, 10.f, -20.f));
	
	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLightComponent->SetupAttachment(FirstPersonCameraComponent);
	
	if (IsValid(SpotLightComponent))
	{
		SpotLightComponent->SetVisibility(false);
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AHorrorSchoolCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AHorrorSchoolCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHorrorSchoolCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHorrorSchoolCharacter::Look);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AHorrorSchoolCharacter::OnInteractStart);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AHorrorSchoolCharacter::OnInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AHorrorSchoolCharacter::OnInteractHold);

		//Use item
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Started, this, &AHorrorSchoolCharacter::UseItem);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AHorrorSchoolCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AHorrorSchoolCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHorrorSchoolCharacter::OnInteractStart()
{
	HoldTimer = 0;
	bHoldTriggered = false;
}

void AHorrorSchoolCharacter::OnInteract()
{
	if (!IsValid(InteractComponent))
		return;

	if (bHoldTriggered)
		return;
	
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	InteractComponent->Interact(CameraLocation, CameraRotation);
}

void AHorrorSchoolCharacter::OnInteractHold()
{
	if (!IsValid(InteractComponent))
		return;
	
	HoldTimer += GetWorld()->GetDeltaSeconds();
	if (HoldTimer >= HoldThreshold)
	{
		bHoldTriggered = true;
		
		FVector CameraLocation;
		FRotator CameraRotation;
		GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

		InteractComponent->InteractHold(CameraLocation, CameraRotation,
			GetWorld()->GetDeltaSeconds());
	}
}

void AHorrorSchoolCharacter::UseItem()
{   
	if (!EquippedItem)
		return;
	
	IUsableInterface::Execute_Usable(EquippedItem.GetObject());
}
