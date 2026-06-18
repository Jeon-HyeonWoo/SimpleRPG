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
class USimpleRPGAttributeSet;

UCLASS()
class SIMPLERPG_API ASimpleRPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ASimpleRPGPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
public:

	USimpleRPGAttributeSet* GetAttributeSet() const { return AttributeSet; }

	void SetPawnData(UPawnData* InPawnData);

	const UPawnData* GetPawnData() const;

public:
	//왜 GE로 안했나? = GE_initStat는 너무 귀찮았다. 런타임 조정일 때만 GE를 써서 특정 AttributeSet을 뽑아 쓰는게 더 나을 것 같다.
	void InitializeStats();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Pawn")
	TObjectPtr<UPawnData> PawnData;

private:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<USimpleRPGAttributeSet> AttributeSet;

};
