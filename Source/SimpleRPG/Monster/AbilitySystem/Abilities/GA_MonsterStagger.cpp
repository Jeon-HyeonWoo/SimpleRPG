// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MonsterStagger.h"
#include "AbilitySystemComponent.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"


UGA_MonsterStagger::UGA_MonsterStagger()
{
	//Instance 정책 : 각 액터별 적용
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_MonsterStagger::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//1. Commit Ability 체크
	if (!CommitAbility(Handle, Actorinfo, ActivationInfo))
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	FGameplayTag ActionTag;
	if (!ExtractActionTag(TriggerEventData, ActionTag))
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	//TaskWaitForStaggerEvent();
	if (!PlayActionMontage(ActionTag))
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* ASC = GetSimpleRPGASC();
	if (ASC)
	{
		FGameplayTagContainer AttackTag;
		AttackTag.AddTag(SimpleRPGGameplayTags::Monster_Ability_Attack);
		ASC->CancelAbilities(&AttackTag, nullptr, this);
	}


}


void UGA_MonsterStagger::TaskWaitForStaggerEvent()
{
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		SimpleRPGGameplayTags::Monster_Event_Stagger,
		nullptr,
		false
	);

	Task->EventReceived.AddDynamic(this, &UGA_MonsterStagger::OnStaggeredAgain);
	Task->ReadyForActivation();
}

void UGA_MonsterStagger::OnStaggeredAgain(FGameplayEventData PayLoad)
{
	FGameplayTag ActionTag;
	if (!ExtractActionTag(&PayLoad, ActionTag))
	{
		return;
	}

	PlayActionMontage(ActionTag);
}

