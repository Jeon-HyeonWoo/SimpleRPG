// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGGameMode.h"
#include "../Character/PawnData.h"
#include "../Player/SimpleRPGPlayerState.h"

ASimpleRPGGameMode::ASimpleRPGGameMode(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	PlayerStateClass = ASimpleRPGPlayerState::StaticClass();
}

void ASimpleRPGGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ASimpleRPGPlayerState* PS = NewPlayer->GetPlayerState<ASimpleRPGPlayerState>();

	if (IsValid(PS))
	{
		if (IsValid(DefaultPawnData))
		{
			PS->PawnData = DefaultPawnData;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%d, %hs, DefaultPawnClass is inValid"), __LINE__, __FUNCTION__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, ASimpleRPGPlayerState is inValid"), __LINE__, __FUNCTION__);
	}


	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

UClass* ASimpleRPGGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	ASimpleRPGPlayerState* PS = InController->GetPlayerState<ASimpleRPGPlayerState>();
	
	if (IsValid(PS))
	{
		UPawnData* PawnData = PS->PawnData;
		if (IsValid(PawnData))
		{
			return PawnData->PawnClass;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%d, %hs, UPawnData Is invalid"), __LINE__, __FUNCTION__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, ASimpleRPGPlayerState is invalid"), __LINE__, __FUNCTION__);
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);

}
