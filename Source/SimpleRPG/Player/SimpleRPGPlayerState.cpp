// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGPlayerState.h"
#include "AbilitySystemComponent.h"

ASimpleRPGPlayerState::ASimpleRPGPlayerState(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	if(IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Log, TEXT("ASC Created Successfully"));
	}
}

UAbilitySystemComponent* ASimpleRPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
};
