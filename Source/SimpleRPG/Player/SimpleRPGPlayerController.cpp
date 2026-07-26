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
#include "SimpleRPG/Teams/SimpleRPGTeamSubsystem.h"

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

FGenericTeamId ASimpleRPGPlayerController::GetGenericTeamId() const
{
	//GetPlayerState -> Call PlayerState::GetTeamId
	ASimpleRPGPlayerState* PS = GetPlayerState<ASimpleRPGPlayerState>();
	if (!PS)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerState Invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return FGenericTeamId::NoTeam;
	}

	return PS->GetTeamId();
}

void ASimpleRPGPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	//GetPlayerState -> Call PlayerState::SetTeamId(FGenericTeamId)
	ASimpleRPGPlayerState* PS = GetPlayerState<ASimpleRPGPlayerState>();
	if (!PS)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerState Invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}
	PS->SetTeamId(NewTeamId);
}

ETeamAttitude::Type ASimpleRPGPlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	UGameInstance* GI = GetGameInstance();
	if (!GI) return ETeamAttitude::Neutral;

	USimpleRPGTeamSubsystem* Sub = GI->GetSubsystem<USimpleRPGTeamSubsystem>();
	if (!Sub) return ETeamAttitude::Neutral;

	return Sub->GetAttitudeBetween(GetGenericTeamId(), Other);

	
	return ETeamAttitude::Type();
}
