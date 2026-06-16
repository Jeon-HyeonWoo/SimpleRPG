// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MonsterStagger.h"
#include "AbilitySystemComponent.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
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

	TaskPlayMontageAndWait();
	TaskWaitForStaggerEvent();
}

void UGA_MonsterStagger::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, Actorinfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_MonsterStagger::TaskPlayMontageAndWait()
{
	if (MontageTask)
	{
		MontageTask->EndTask();
	}


	ASimpleRPGMonsterBase* Pawn = Cast<ASimpleRPGMonsterBase>(GetAvatarActorFromActorInfo());
	if (!IsValid(Pawn))
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerPawn is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//이 부분 Tag를 어떻게 채워넣을지?
	UAnimMontage* Montage = Pawn->GetAnimSet().GetAnimMontageByTag(Pawn->HitReactTag);
	UE_LOG(LogTemp, Warning, TEXT("Stagger Montage: %s"), *GetNameSafe(Montage));

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage,
		1.0f
	);

	//Task->OnBlendOut.AddDynamic(this, &UGA_MonsterStagger::OnMontageCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_MonsterStagger::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_MonsterStagger::OnMontageInterupted);
	Task->OnCancelled.AddDynamic(this, &UGA_MonsterStagger::OnMontageCancelled);

	Task->ReadyForActivation();

	MontageTask = Task;
}

void UGA_MonsterStagger::TaskWaitForStaggerEvent()
{
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		StaggerEventTag,
		nullptr,
		false
	);

	Task->EventReceived.AddDynamic(this, &UGA_MonsterStagger::OnStaggeredAgain);
	Task->ReadyForActivation();
}


void UGA_MonsterStagger::OnMontageCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("Stagger Montage COMPLETED"));
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false
	);
}

void UGA_MonsterStagger::OnMontageCancelled()
{
	UE_LOG(LogTemp, Warning, TEXT("Stagger Montage CANCELLED"));
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		true
	);
}

void UGA_MonsterStagger::OnMontageInterupted()
{
	UE_LOG(LogTemp, Warning, TEXT("Stagger Montage INTERUPTED"));
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		true
	);
}

void UGA_MonsterStagger::OnStaggeredAgain(FGameplayEventData PayLoad)
{
	TaskPlayMontageAndWait();
}

