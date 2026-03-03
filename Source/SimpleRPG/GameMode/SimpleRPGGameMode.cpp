// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGGameMode.h"
#include "../Player/SimpleRPGPlayerState.h"

ASimpleRPGGameMode::ASimpleRPGGameMode(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	PlayerStateClass = ASimpleRPGPlayerState::StaticClass();
}
