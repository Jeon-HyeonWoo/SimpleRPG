// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/SimpleRPGCharacterBase.h"
#include "SimpleRPGMonsterBase.generated.h"

class UAttributeSet;
class USimpleRPGMonsterAttributeSet;
class UMonsterData;
class UWidgetComponent;
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
	//Getter, Setter

	USimpleRPGMonsterAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UMonsterData* GetMonsterData() { return MonsterData; }

public:
	//Binding, Handle Function

	void HandleDamaged(AActor* _Instigator);

	UFUNCTION()
	void HandleDeath(AActor* Actor);
	
	void UpdateHPBar();

public:
	//Rel. Stat, Status Function
	
	void RestoreHP();

protected:
	//Helper Function

	void GiveAbility();
	void InitializeStats();
	void BindDelegate();
	void ReportDamageToPerception(AActor* _Instigator);

public:

	UPROPERTY()
	TObjectPtr<USimpleRPGMonsterAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG")
	TObjectPtr<UMonsterData> MonsterData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG")
	TObjectPtr<UWidgetComponent> HPBarWidgetComponent;

private:

	bool bIsDead = false;
};
