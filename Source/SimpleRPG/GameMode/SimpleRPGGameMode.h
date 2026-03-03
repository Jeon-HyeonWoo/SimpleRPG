// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpleRPGGameMode.generated.h"

/**
 * 
 */

class ASimpleRPGPlayerState;

UCLASS()
class SIMPLERPG_API ASimpleRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ASimpleRPGGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
