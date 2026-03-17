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
class USimpleRPGInputConfig;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SIMPLERPG_API ASimpleRPGPlayerCharacter : public ASimpleRPGCharacterBase
{
	GENERATED_BODY()
	
public:

	ASimpleRPGPlayerCharacter();

	/* override from ACharacter */
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
//Components
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Animation")
	TSubclassOf<UAnimInstance> UnArmedAnimLayer;


//Native Input Handler Func
public:

	void MoveHandler(const FInputActionValue& Value);
	void LookHandler(const FInputActionValue& Value);

//Native Input
public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TObjectPtr<USimpleRPGInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TObjectPtr<UInputAction> IA_Look;
};
