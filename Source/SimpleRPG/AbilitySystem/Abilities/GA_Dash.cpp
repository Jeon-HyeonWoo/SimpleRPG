// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dash.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../AbilityTask/AT_DashMove.h"
#include "../../Character/SimpleRPGPlayerCharacter.h"

UGA_Dash::UGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dashing")));

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dashing")));
}

void UGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, Actorinfo, ActivationInfo))
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	//이동 제한 GE 적용
	if (BlockMovementEffect)
	{
		BlockMovementEffectHandle = ApplyGameplayEffectToOwner(
			Handle,
			Actorinfo,
			ActivationInfo,
			BlockMovementEffect.GetDefaultObject(),
			1.0f
		);
	}

	if (DashMontage)
	{
		DashDuration = DashMontage->GetPlayLength();

		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			DashMontage,
			2.0f
		);

		MontageTask->OnCompleted.AddDynamic(this, &UGA_Dash::OnMontageCompleted);
		//MontageTask->OnBlendOut.AddDynamic(this, &UGA_Dash::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Dash::OnMontageCancelled);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Dash::OnMontageCancelled);

		MontageTask->ReadyForActivation();
	}


	/* Owning Ability, Direction Vector, Distance, Duration */
	UAT_DashMove* AT_DashMove = UAT_DashMove::CreateDashMove(this, GetDashDirection(), DashDistance, DashDuration);
	if (AT_DashMove)
	{
		AT_DashMove->ReadyForActivation();
	}
}

void UGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (BlockMovementEffectHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(BlockMovementEffectHandle);
	}

	Super::EndAbility(Handle, Actorinfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UGameplayEffect* UGA_Dash::GetCooldownGameplayEffect() const
{
	return CoolDownEffect.GetDefaultObject();
}

void UGA_Dash::OnMontageCompleted()
{
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false
	);
}

void UGA_Dash::OnMontageCancelled()
{
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		true
	);
}

FVector UGA_Dash::GetDashDirection() const
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		return FVector::ZeroVector;
	}

	ASimpleRPGPlayerCharacter* PlayerCharacter = Cast<ASimpleRPGPlayerCharacter>(Character);
	if (!PlayerCharacter)
	{
		return Character->GetActorForwardVector();
	}

	FVector LastInput = PlayerCharacter->GetLastInputDirection();

	float TimeSinceInput = GetWorld()->GetTimeSeconds() - PlayerCharacter->GetLastInputTime();

	if (TimeSinceInput > 0.1f || LastInput.IsNearlyZero())
	{
		return Character->GetActorForwardVector();
	}

	return LastInput.GetSafeNormal();
}
