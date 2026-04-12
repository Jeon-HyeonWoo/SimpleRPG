// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dash.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	FVector DashDirection = GetDashDirection();

	if (!DashDirection.IsNearlyZero())
	{
		FRotator DashRotation = DashDirection.Rotation();
		Character->SetActorRotation(FRotator(0.0f, DashRotation.Yaw, 0.0f));
	}

	if (DashMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			DashMontage
		);

		MontageTask->OnCompleted.AddDynamic(this, &UGA_Dash::OnMontageCompleted);
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_Dash::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Dash::OnMontageCancelled);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Dash::OnMontageCancelled);

		MontageTask->ReadyForActivation();
	}

	float DashSpeed = DashDistance / DashDuration;
	Character->LaunchCharacter(DashDirection * DashSpeed, true, true);

}

void UGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (BlockMovementEffectHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(BlockMovementEffectHandle);
	}

	Super::EndAbility(Handle, Actorinfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

	AController* Controller = Character->GetController();

	if (!Controller)
	{
		return Character->GetActorForwardVector();
	}

	FRotator ControlRotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();

	if (!MovementComp)
	{
		return Character->GetActorForwardVector();
	}

	FVector LastInput = MovementComp->GetLastInputVector();

	if (LastInput.IsNearlyZero())
	{
		return Character->GetActorForwardVector();
	}

	return LastInput.GetSafeNormal();
}
