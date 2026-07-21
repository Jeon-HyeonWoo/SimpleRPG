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
	UE_LOG(LogTemp, Warning, TEXT("%d, %hs"), __LINE__, __FUNCTION__);
#pragma region valid check
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ASimpleRPGMonsterBase* Pawn = Cast<ASimpleRPGMonsterBase>(GetAvatarActorFromActorInfo());
	if (!ensureMsgf(IsValid(Pawn), TEXT("MonsterDeath : Pawn invalid")))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}	
	

	AAIController* AIController = Cast<AAIController>(Pawn->GetController());
	if (!ensureMsgf(IsValid(AIController), TEXT("MonsterDeath : AIController invalid")))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	UBrainComponent* BrainComp = AIController->GetBrainComponent();
	if (!ensureMsgf(IsValid(BrainComp), TEXT("MonsterDeath : BrainComp invalid")))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

#pragma endregion

	FGameplayTag ActionTag;
	if (!ExtractActionTag(TriggerEventData, ActionTag))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	//CharacterMovement Stop
	Pawn->StopMovement();
	//Behavior Tree Stop
	BrainComp->StopLogic(TEXT("Death"));

	if (!PlayActionMontage(ActionTag))
	{
		UE_LOG(LogTemp, Error, TEXT("DeathMontage playing failed : %s"), *GetNameSafe(Pawn));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		Pawn->Destroy();
		return;
	}

	if (MontageTask)
	{
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_MonsterDeath::OnDeathBlendOut);
	}
}

void UGA_MonsterDeath::OnDeathBlendOut()
{
	ASimpleRPGMonsterBase* Monster = GetOwningMonster();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	if (IsValid(Monster))
	{
		Monster->Destroy();
	}
}

void UGA_MonsterDeath::OnActionMontageCompleted()
{
	ASimpleRPGMonsterBase* Monster = GetOwningMonster();

	Super::OnActionMontageCompleted();

	if (IsValid(Monster)) { Monster->Destroy(); }
}
