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

	if (BlockMovementEffect)
	{
		BlockMovementEffectHandle = ApplyGameplayEffectToOwner(
			Handle, Actorinfo, ActivationInfo, BlockMovementEffect.GetDefaultObject(), 1.0f);
	}

	PlayMontage();
	WaitInputRelease();
	DamageEventTask();
}

void UGA_SwordChargeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (BlockMovementEffectHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(BlockMovementEffectHandle);
	}

	Super::EndAbility(Handle, Actorinfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UGameplayEffect* UGA_SwordChargeAttack::GetCooldownGameplayEffect() const
{
	return CoolDownEffect.GetDefaultObject();
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
	UE_LOG(LogTemp, Warning, TEXT("WaitInputReleaseCallBack : TimeHeld = %f"), TimeHeld);

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
	FName RowName = bIsFullCharge ? FName("Sword_ChargeAttack_Full") : FName("Sword_ChargeAttack_Under");

	float Ratio;

	if (!GetSkillRatio(DamageDataTable, RowName, Ratio)) return;

	ApplyDamageToTarget(const_cast<AActor*>(PayLoad.Target.Get()), DamageEffect, Ratio);
}
