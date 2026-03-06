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
class UAnimInstance;

UCLASS()
class SIMPLERPG_API ASimpleRPGPlayerCharacter : public ASimpleRPGCharacterBase
{
	GENERATED_BODY()
	
public:

	ASimpleRPGPlayerCharacter();

	/* override from ACharacter */
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Animation")
	TSubclassOf<UAnimInstance> UnArmedAnimLayer;
};
