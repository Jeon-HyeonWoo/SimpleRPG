// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_PlayerDeath.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"

UGA_PlayerDeath::UGA_PlayerDeath()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(SimpleRPGGameplayTags::Ability_Death);
}

void UGA_PlayerDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, Actorinfo, ActivationInfo))
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Player Death!"));

	EndAbility(Handle, Actorinfo, ActivationInfo, true, false);
}

void UGA_PlayerDeath::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, Actorinfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
