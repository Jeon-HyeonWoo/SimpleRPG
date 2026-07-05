// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/SimpleRPGCharacterBase.h"
#include "GameplayEffect.h"
#include "SimpleRPG/Monster/Data/MonsterAnimSet.h"
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

	ASimpleRPGMonsterBase(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

public:
	//Getter, Setter

	USimpleRPGMonsterAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UMonsterData* GetMonsterData() { return MonsterData; }
	
	const FMonsterAnimSet& GetAnimSet() const { return MonsterAnimSet; }

public:
	//Binding, Handle Function

	void HandleDamaged(AActor* _Instigator);

	UFUNCTION()
	void HandleDeath(AActor* Actor);
	
	void UpdateHPBar();

	void StopMovement();

public:
	//Rel. Stat, Status Function
	
	void RestoreHP();
	void ApplyInvulnerability(); 
	void RemoveInvulnerability();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> InvulnerabilityEffect;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Anim")
	FMonsterAnimSet MonsterAnimSet;

public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Anim")
	FGameplayTag HitReactTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Anim")
	FGameplayTag DeathTag;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Event")
	FGameplayTag StaggerEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Event")
	FGameplayTag DeathEventTag;
private:

	UPROPERTY()
	FActiveGameplayEffectHandle InvulnerabilityEffectHandle;

private:

	bool bIsDead = false;
};
