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
	//Other에서 TeamInterface 찾기
	//Other가 Pawn이면 Other.GetPlayerController() 에서 찾기
	//못 찾으면 Netural
	//상대 팀 번호 얻기
	// 내 팀 번호와 비교
	// 같으면 Friendly, 다르면 Hostile
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (!OtherPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("OtherPawn invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return ETeamAttitude::Neutral;
	}

	AController* OtherController = OtherPawn->GetController();
	if (!OtherController)
	{
		return ETeamAttitude::Neutral;
	}

	const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherController);
	if (!TeamAgent)
	{
		TeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
		if (!TeamAgent)
		{
			return ETeamAttitude::Neutral;
		}
	}
	
	FGenericTeamId OtherTeamId = TeamAgent->GetGenericTeamId();

	
	return ETeamAttitude::Type();
}
