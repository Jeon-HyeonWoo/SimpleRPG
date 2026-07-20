// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterMovementComponent.h"
#include "SimpleRPG/Character/SimpleRPGPlayerCharacter.h"
#include "SimpleRPG/AbilitySystem/SimpleRPGAttributeSet.h"
#include "AbilitySystemComponent.h"

float UPlayerCharacterMovementComponent::GetMaxSpeed() const
{
	//1. GetOwner's ASI
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner());
	if (!ASI)
	{
		return Super::GetMaxSpeed();
	}

	//2. GetOwner's ASC
	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC)
	{
		return Super::GetMaxSpeed();
	}

	//3. Get Attribute
	float Base = ASC->GetNumericAttribute(USimpleRPGAttributeSet::GetBaseMoveSpeedAttribute());
	float Mult = ASC->GetNumericAttribute(USimpleRPGAttributeSet::GetMoveSpeedMultiplierAttribute());
	return 	Base * Mult;
}
