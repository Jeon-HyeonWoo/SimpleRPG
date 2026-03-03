// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGPlayerCharacter.h"
#include "../Player/SimpleRPGPlayerState.h"


void ASimpleRPGPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	/*
	* Connecting CharacterBase's ASC to PlayerState's ASC
	*/
	ASimpleRPGPlayerState* SimpleRPGPlayerState = Cast<ASimpleRPGPlayerState>(GetPlayerState());

	if (IsValid(SimpleRPGPlayerState))
	{
		AbilitySystemComponent = SimpleRPGPlayerState->GetAbilitySystemComponent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerState is invalid"));
	}

	
}
