// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MonsterMeleeAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGA_MonsterMeleeAttack::UGA_MonsterMeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_MonsterMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//CommitAbility 체크
	if (!CommitAbility(Handle, Actorinfo, ActivationInfo))
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	//AttackMontageDataArray 배열 유효성 체크
	if (AttackMontageDataArray.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("AttackMontageDataArry is Empty : %d, %hs"), __LINE__, __FUNCTION__);
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	//공격 중 이동 불가 Effect 설정
	if (BlockMovementEffect)
	{
		BlockMovementEffectHandle = ApplyGameplayEffectToOwner(
			Handle, Actorinfo, ActivationInfo, BlockMovementEffect.GetDefaultObject(), 1.0f);
	}

	//공격 Montage 랜덤 재생 Index설정
	MontageDataIndex = FMath::RandRange(0, AttackMontageDataArray.Num() - 1);

	PlayMontage();
	WaitForHitEvent();
}

void UGA_MonsterMeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
}

void UGA_MonsterMeleeAttack::PlayMontage()
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackMontageDataArray[MontageDataIndex].AnimMontage,
		1.0f
	);

	Task->OnBlendOut.AddDynamic(this, &UGA_MonsterMeleeAttack::OnMontageCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_MonsterMeleeAttack::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_MonsterMeleeAttack::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_MonsterMeleeAttack::OnMontageCancelled);
	
	Task->ReadyForActivation();
}

void UGA_MonsterMeleeAttack::WaitForHitEvent()
{
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		HitEventTag
	);

	Task->EventReceived.AddDynamic(this, &UGA_MonsterMeleeAttack::OnHitEventReceived);


	Task->ReadyForActivation();
}

void UGA_MonsterMeleeAttack::OnHitEventReceived(FGameplayEventData PayLoad)
{
}

void UGA_MonsterMeleeAttack::OnMontageCompleted()
{
}

void UGA_MonsterMeleeAttack::OnMontageCancelled()
{
}
