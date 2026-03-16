// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "SimpleRPGPlayerState.h"
#include "../Character/PawnData.h"
#include "../Input/SimpleRPGInputConfig.h"

ASimpleRPGPlayerController::ASimpleRPGPlayerController()
{

}

void ASimpleRPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = -40.0f;
		PlayerCameraManager->ViewPitchMax = 20.0f;
	}

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(InputMappingContext, InputMappingContextPriority);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, EnhancedInputSubsystem is invalid"), __LINE__, __FUNCTION__);
	}

	
}

void ASimpleRPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	if (!(IsValid(EnhancedInput))) return;
	
	EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASimpleRPGPlayerController::Move);
	EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASimpleRPGPlayerController::Look);

}

void ASimpleRPGPlayerController::Move(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn<APawn>();
	
	if (ControlledPawn)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);

		if (MovementVector.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			ControlledPawn->AddMovementInput(MovementDirection, MovementVector.X);
		}

		if (MovementVector.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			ControlledPawn->AddMovementInput(MovementDirection, MovementVector.Y);
		}
	}

}

void ASimpleRPGPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (LookVector.X != 0.0f)
	{
		AddYawInput(LookVector.X);
	}
	 
	if (LookVector.Y != 0.0f)
	{
		AddPitchInput(-LookVector.Y);
	}
}
