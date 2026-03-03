// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGCharacterBase.h"
#include "SimpleRPGPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API ASimpleRPGPlayerCharacter : public ASimpleRPGCharacterBase
{
	GENERATED_BODY()
	
public:

	/* override from ACharacter */
	virtual void PossessedBy(AController* NewController) override;
};
