// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGCharacterBase.h"
#include "SimpleRPGPlayerCharacter.generated.h"

/**
 * 
 */
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SIMPLERPG_API ASimpleRPGPlayerCharacter : public ASimpleRPGCharacterBase
{
	GENERATED_BODY()
	
public:

	ASimpleRPGPlayerCharacter();

	/* override from ACharacter */
	virtual void PossessedBy(AController* NewController) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
};
