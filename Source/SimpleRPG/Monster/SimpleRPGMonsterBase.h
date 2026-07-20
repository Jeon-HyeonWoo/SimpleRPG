// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/SimpleRPGCharacterBase.h"
#include "GameplayEffect.h"
#include "SimpleRPGMonsterBase.generated.h"

class UAttributeSet;
class USimpleRPGMonsterAttributeSet;
class UMonsterData;
class UWidgetComponent;
class UMonsterHealthComponent;
/**
 * 
 */
UCLASS()
class SIMPLERPG_API ASimpleRPGMonsterBase : public ASimpleRPGCharacterBase
{
	GENERATED_BODY()
	
public:

	ASimpleRPGMonsterBase(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

public:
	//Getter, Setter

	USimpleRPGMonsterAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UMonsterData* GetMonsterData() { return MonsterData; }

	UMonsterHealthComponent* GetMonsterHealthComponent() const { return MonsterHealthComponent; }

public:

	void StopMovement();

public:
	
	void ApplyInvulnerability(); 
	void RemoveInvulnerability();

protected:

	void GiveAbility();
	void InitializeStats();

public:

	UPROPERTY()
	TObjectPtr<USimpleRPGMonsterAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG")
	TObjectPtr<UMonsterData> MonsterData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Component")
	TObjectPtr<UWidgetComponent> HPBarWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Component")
	TObjectPtr<UMonsterHealthComponent> MonsterHealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> InvulnerabilityEffect;

	

private:

	UPROPERTY()
	FActiveGameplayEffectHandle InvulnerabilityEffectHandle;

private:

	bool bIsDead = false;
};
