// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGCharacterBase.h"
#include "GameplayTagContainer.h"
#include "SimpleRPGPlayerCharacter.generated.h"

/**
 * 
 */
class USpringArmComponent;
class UCameraComponent;
class UAnimInstance;
class USimpleRPGInputConfig;
class USimpleRPGEquipmentComponent;
class UInputAction;
class ASimpleRPGPlayerState;
class UPawnData;
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
	
public:
	//Helper Function

	//Initialze Helper Function
	void InitializeAbilitySystem(ASimpleRPGPlayerState* PS);
	void GrantDefaultAbilities(const UPawnData* PawnData);
	void InitializeEquipment(const UPawnData* PawnData);

//Components
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Equipment")
	TObjectPtr<USimpleRPGEquipmentComponent> EquipmentComponent;


//Native Input Handler Func
public:

	void MoveHandler(const FInputActionValue& Value, FGameplayTag InputTag);
	void LookHandler(const FInputActionValue& Value, FGameplayTag InputTag);
	void WeaponslotHandler(const FInputActionValue& Value, FGameplayTag InputTag);

//Ability Input Handler Func
public:

	void OnAbilityInputPressed(const FInputActionValue& Value, FGameplayTag InputTag);
	void OnAbilityInputReleased(const FInputActionValue& Value, FGameplayTag InputTag);

//Native Input
public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TObjectPtr<USimpleRPGInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TObjectPtr<UInputAction> IA_Look;


public:

	FVector GetLastInputDirection() const { return LastInputDirection; }
	float GetLastInputTime() const { return LastInputTime; }
private:

	UPROPERTY()
	FVector LastInputDirection;

	UPROPERTY()
	float LastInputTime;
};
