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

}
