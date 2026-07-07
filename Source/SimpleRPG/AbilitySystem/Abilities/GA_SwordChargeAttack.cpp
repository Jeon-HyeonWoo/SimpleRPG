// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_SwordChargeAttack.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../Data/DamageDataTableRow.h"

UGA_SwordChargeAttack::UGA_SwordChargeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Ability 정체성
	AbilityTags.AddTag(SimpleRPGGameplayTags::Ability_Attack_Sword);
	
	//활성 중 차단
	BlockAbilitiesWithTag.AddTag(SimpleRPGGameplayTags::Ability_Attack);
	BlockAbilitiesWithTag.AddTag(SimpleRPGGameplayTags::Ability_WeaponSwap);
}

void UGA_SwordChargeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
	PlayMontage();
	WaitInputRelease();
	DamageEventTask();
}

void UGA_SwordChargeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveBlockMovement();
	Super::EndAbility(Handle, Actorinfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_SwordChargeAttack::PlayMontage()
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,					//OwnerAbility
		NAME_None,				//Name
		ChargeAttackMontage,	//Subclass
		1.0f,					//PlayRate
		SectionNames[0]			//Start Section Name
	);

	Task->OnBlendOut.AddDynamic(this, &UGA_SwordChargeAttack::OnMontageCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_SwordChargeAttack::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_SwordChargeAttack::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_SwordChargeAttack::OnMontageCancelled);

	Task->ReadyForActivation();
}

void UGA_SwordChargeAttack::WaitInputRelease()
{
	UAbilityTask_WaitInputRelease* Task = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);

	Task->OnRelease.AddDynamic(this, &UGA_SwordChargeAttack::WaitInputReleaseCallBack);

	Task->ReadyForActivation();
}

void UGA_SwordChargeAttack::WaitInputReleaseCallBack(float TimeHeld)
{
	if (TimeHeld >= FullChargeTime)
	{
		bIsFullCharge = true;
		FullChargeAttackEffectHandle = ApplyGameplayEffectToOwner(
			CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, FullChargeAttackEffect.GetDefaultObject(), 1.0, 1
		);
	}
	else
	{
		bIsFullCharge = false;
		UnderChargeAttackEffectHandle = ApplyGameplayEffectToOwner(
			CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, UnderChargeAttackEffect.GetDefaultObject(), 1.0f, 1
		);
	}

	GetAbilitySystemComponentFromActorInfo()->CurrentMontageJumpToSection(SectionNames[2]);
}

void UGA_SwordChargeAttack::OnMontageCompleted()
{
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false
	);
}

void UGA_SwordChargeAttack::OnMontageCancelled()
{
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		true
	);
}

void UGA_SwordChargeAttack::DamageEventTask()
{
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		SimpleRPGGameplayTags::Event_Combat_HitDetect
	);

	Task->EventReceived.AddDynamic(this, &UGA_SwordChargeAttack::OnDamageEvent);

	Task->ReadyForActivation();
}

void UGA_SwordChargeAttack::OnDamageEvent(FGameplayEventData PayLoad)
{
	const FDataTableRowHandle& DamageRow = bIsFullCharge ? FullChargeRow : UnderChargeRow;

	float Ratio;

	if (!GetSkillRatio(DamageRow, Ratio)) return;

	ApplyDamageToTarget(const_cast<AActor*>(PayLoad.Target.Get()), DamageEffect, Ratio);
}
