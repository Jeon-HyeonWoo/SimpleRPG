// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_SwordPowerUp.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"



UGA_SwordPowerUp::UGA_SwordPowerUp()
{
	/* 객체 별 정책 = 공유 액터 없이 한 액터당 하나 씩 부여 */
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Ability 정체성
	AbilityTags.AddTag(SimpleRPGGameplayTags::Ability_Attack_Sword);

	//활성 중 차단
	BlockAbilitiesWithTag.AddTag(SimpleRPGGameplayTags::Ability_Attack);
	BlockAbilitiesWithTag.AddTag(SimpleRPGGameplayTags::Ability_WeaponSwap);
}

void UGA_SwordPowerUp::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, Actorinfo, ActivationInfo))
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	if (!AvatarActor)
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}
	ApplyBlockMovement();
	PlayPowerUpMontage();
	PowerUpEvent();
}

void UGA_SwordPowerUp::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveBlockMovement();
	Super::EndAbility(Handle, Actorinfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_SwordPowerUp::PlayPowerUpMontage()
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, //OwningAbility
		NAME_None,
		PowerUpMontage,
		1.0f
	);

	//Task->OnBlendOut.AddDynamic(this, &UGA_SwordPowerUp::OnMontageCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_SwordPowerUp::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_SwordPowerUp::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_SwordPowerUp::OnMontageCancelled);

	Task->ReadyForActivation();
}

void UGA_SwordPowerUp::PowerUpEvent()
{
	UAbilityTask_WaitGameplayEvent* Task =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, SimpleRPGGameplayTags::Event_Skill_Sword_PowerUp);

	Task->EventReceived.AddDynamic(this, &UGA_SwordPowerUp::OnPowerUpActivated);

	Task->ReadyForActivation();
}

void UGA_SwordPowerUp::OnMontageCompleted()
{
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false
	);
}

void UGA_SwordPowerUp::OnMontageCancelled()
{
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		true
	);
}

void UGA_SwordPowerUp::OnPowerUpActivated(FGameplayEventData PayLoad)
{
	ApplyGameplayEffectToOwner(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		BuffEffect.GetDefaultObject(),
		1.0f
	);
}
