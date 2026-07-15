// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MonsterMeleeAttack.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SimpleRPG/Monster/Data/MonsterDamageDataTableRow.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"

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

	//TriggerEventData 체크
	if (!TriggerEventData)
	{
		UE_LOG(LogTemp, Error, TEXT("TriggetEvent nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	if (TriggerEventData->InstigatorTags.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("TriggetEvent is empty : %d, %hs"), __LINE__, __FUNCTION__);
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	//InstigatorTags 확인
	FGameplayTag ActionTag = TriggerEventData->InstigatorTags.First();
	if (!ActionTag.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("ActionTag invalid : %d, %hs"), __LINE__, __FUNCTION__);
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	ApplyBlockMovement();
	WaitForHitEvent();
	
	if (!PlayActionMontage(ActionTag))
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}
}

void UGA_MonsterMeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveBlockMovement();
	Super::EndAbility(Handle, Actorinfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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
	//CurrentEntry Check
	if (!CurrentEntry)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentEntry invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//Target(Player)가져오기
	AActor* TargetActor = const_cast<AActor*>(PayLoad.Target.Get());
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//Target ASC 가져오기
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target ASC is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//GetRow, nullptr check
	FMonsterDamageDataTableRow* Row = CurrentEntry->DamageRowHandle.GetRow<FMonsterDamageDataTableRow>(TEXT(""));
	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("DTRow is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//SpecHandle 만들기
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect, GetAbilityLevel());
	UE_LOG(LogTemp, Warning, TEXT("Ratio: %f"), Ratio);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(
		SimpleRPGGameplayTags::SetByCaller_Monster_Ratio, Ratio);
	
	

	//Test Log
	UE_LOG(LogTemp, Warning, TEXT("Applying Damage to Target : %s"), *GetNameSafe(TargetActor));

	//Target에 SpecHandle Data값 적용(MonsterCalcExec가 최종적으로 계산하여 Target의 AttributeSet 값 조절)
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(),
		TargetASC
	);
}

