// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HorrorSchoolCharacter.generated.h"

class USpotLightComponent;
class IUsableInterface;
class UInteractComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AHorrorSchoolCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	//Interact Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	//Input to use the item which the player hold
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UseItemAction;
	
	// Input to sprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;
	
	//Input for the Menu
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MenuAction;

public:
	AHorrorSchoolCharacter();

	//Return the Item Attachment scene
	TObjectPtr<USceneComponent> GetItemAttachment() const { return ItemAttachment; }

	//Return and Set the item which the player hold
	TScriptInterface<IUsableInterface> GetEquippedItem() const {return EquippedItem; };
	void SetEquippedItem(TScriptInterface<IUsableInterface> InEquippedItem) {EquippedItem = InEquippedItem; };
	
	// Return the SpotLight for the flashlight
	USpotLightComponent* GetSpotLightComponent() const { return SpotLightComponent; }

protected:
	virtual void BeginPlay() override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	virtual void Tick(float DeltaSeconds) override;
	
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


protected:
	//Where the item will be attached
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment", meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> ItemAttachment;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flashlight")
	USpotLightComponent* SpotLightComponent;

	//Reference to the object which the player hold
	TScriptInterface<IUsableInterface> EquippedItem;
	
public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInteractComponent> InteractComponent;

	UPROPERTY(EditAnywhere, Category="Input|Hold", meta=(AllowPrivateAccess))
	float HoldThreshold = 5.0f;

	float HoldTimer = 0.f;
	bool bHoldTriggered = false;
	
	float NormalSpeed;
	
	UPROPERTY(EditAnywhere, Category="Player|Sprint")
	float SprintSpeed;
	
	UFUNCTION()
	void OnInteract();

	UFUNCTION()
	void OnInteractHold();

	UFUNCTION()
	void OnInteractStart();
	
	UFUNCTION()
	void OnInteractRelease();

	UFUNCTION()
	void UseItem();
	
	UFUNCTION()
	void Sprint();
	
	UFUNCTION()
	void Walk();
	
	UFUNCTION()
	void OpenMenu();
};  

