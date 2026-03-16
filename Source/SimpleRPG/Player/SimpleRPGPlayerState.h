// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"

#include "SimpleRPGPlayerState.generated.h"

/**
 * 
 */

class UPawnData;
class UAbilitySystemComponent;

UCLASS()
class SIMPLERPG_API ASimpleRPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ASimpleRPGPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
public:

	void SetPawnData(UPawnData* InPawnData);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Pawn")
	TObjectPtr<UPawnData> PawnData;

private:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

};
