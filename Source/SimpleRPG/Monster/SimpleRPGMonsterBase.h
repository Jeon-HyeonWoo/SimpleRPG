// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/SimpleRPGCharacterBase.h"
#include "SimpleRPGMonsterBase.generated.h"

class UAttributeSet;
class USimpleRPGMonsterAttributeSet;
/**
 * 
 */
UCLASS()
class SIMPLERPG_API ASimpleRPGMonsterBase : public ASimpleRPGCharacterBase
{
	GENERATED_BODY()
	
public:

	ASimpleRPGMonsterBase();

	virtual void PossessedBy(AController* NewController) override;

public:

	USimpleRPGMonsterAttributeSet* GetAttributeSet() const { return AttributeSet; }

public:

	UPROPERTY()
	TObjectPtr<USimpleRPGMonsterAttributeSet> AttributeSet;

};
