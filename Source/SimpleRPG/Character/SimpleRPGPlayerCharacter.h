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
class UPlayerHealthComponent;
struct FInputActionValue;

UCLASS()
class SIMPLERPG_API ASimpleRPGPlayerCharacter : public ASimpleRPGCharacterBase
{
	GENERATED_BODY()
	
public:

	ASimpleRPGPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	/* override from ACharacter */
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Equipment")
	TObjectPtr<UPlayerHealthComponent> HealthComponent;

//Native Input Handler Func
public:

	void MoveHandler(const FInputActionValue& Value, FGameplayTag InputTag);
	void LookHandler(const FInputActionValue& Value, FGameplayTag InputTag);
	void WeaponslotHandler(const FInputActionValue& Value, FGameplayTag InputTag);

//Ability Input Handler Func
public:

	void OnAbilityInputPressed(const FInputActionValue& Value, FGameplayTag InputTag);
	void OnAbilityInputReleased(const FInputActionValue& Value, FGameplayTag InputTag);

public:

	FVector GetLastInputDirection() const { return LastInputDirection; }
	float GetLastInputTime() const { return LastInputTime; }

//Native Input
public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TObjectPtr<USimpleRPGInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TObjectPtr<UInputAction> IA_Look;

private:

	UPROPERTY()
	FVector LastInputDirection;

	UPROPERTY()
	float LastInputTime;
};
