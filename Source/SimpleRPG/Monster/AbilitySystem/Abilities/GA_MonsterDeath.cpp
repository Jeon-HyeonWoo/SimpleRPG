// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MonsterDeath.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "AIController.h"
#include "BrainComponent.h"

UGA_MonsterDeath::UGA_MonsterDeath()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_MonsterDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ASimpleRPGMonsterBase* Pawn = Cast<ASimpleRPGMonsterBase>(GetAvatarActorFromActorInfo());
	if (Pawn)
	{
		Pawn->StopMovement();
	}

	if (AAIController* AIController = Cast<AAIController>(Pawn->GetController()))
	{
		if (UBrainComponent* Brain = AIController->GetBrainComponent())
		{
			Brain->StopLogic(TEXT("Death"));
		}
	}

	TaskPlayDeathMontage();
}

void UGA_MonsterDeath::TaskPlayDeathMontage()
{
	UAnimMontage* Montage = GetDeathMontage();
	if (!Montage)
	{
		UE_LOG(LogTemp, Error, TEXT("Montage is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage,
		1.0f
	);

	
	Task->OnCompleted.AddDynamic(this, &UGA_MonsterDeath::OnMontageCompleted);

	Task->ReadyForActivation();
}

UAnimMontage* UGA_MonsterDeath::GetDeathMontage()
{
	ASimpleRPGMonsterBase* Pawn = Cast<ASimpleRPGMonsterBase>(GetAvatarActorFromActorInfo());
	if (!IsValid(Pawn))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterPawn is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}
	
	const FGameplayTag& Tag = Pawn->DeathTag;
	if (!Tag.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterDeathTag is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	UAnimMontage* Montage = Pawn->GetAnimSet().GetAnimMontageByTag(Pawn->DeathTag);
	if (Montage)
	{
		return Montage;
	}

	return nullptr;
}

void UGA_MonsterDeath::OnMontageCompleted()
{
	ASimpleRPGMonsterBase* Pawn = Cast<ASimpleRPGMonsterBase>(GetAvatarActorFromActorInfo());

	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false
	);

	if (IsValid(Pawn))
	{
		Pawn->Destroy();
	}
	
}
