// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterHealthComponent.generated.h"

class ASimpleRPGMonsterBase;
class USimpleRPGMonsterAttributeSet;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLERPG_API UMonsterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterHealthComponent();

	void InitializeWithOwner(ASimpleRPGMonsterBase* InMonster);

public:

	UFUNCTION()
	void HandleDamaged(AActor* _Instigator);

	UFUNCTION()
	void HandleDeath(AActor* Actor);

	void RestoreHP();

	//TODO : Move to UIComponent
	void UpdateHPBar();

private:

	void BindDelegate();

	void ReportDamageToPerception(AActor* _Instigator);

	USimpleRPGMonsterAttributeSet* GetMonsterAS() const;

private:

	TWeakObjectPtr<ASimpleRPGMonsterBase> MonsterCharacter;

	
	bool bIsDead = false;
};
