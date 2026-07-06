// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dash.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Character/SimpleRPGPlayerCharacter.h"


UGA_Dash::UGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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

	ApplyBlockMovement();

	PlayMontage();
}

void UGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveBlockMovement();
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

EDashDirection UGA_Dash::GetDashDirectionEnum() const
{
	ASimpleRPGPlayerCharacter* PlayerCharacter = Cast<ASimpleRPGPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter)
	{
		return EDashDirection::Forward;
	}

	FVector InputDir = PlayerCharacter->GetLastInputDirection().GetSafeNormal();
	
	if (InputDir.IsNearlyZero())
	{
		return EDashDirection::Backward;
	}

	FVector Forward = PlayerCharacter->GetActorForwardVector();
	
	//Dot value : -1(back), 0(90), 1(front)
	float Dot = FVector::DotProduct(Forward, InputDir);
	//Cross Value = Cross.Z < 0 (left) Cross.Z > 0 (right)
	FVector Cross = FVector::CrossProduct(Forward, InputDir);


	float AngleRad = FMath::Atan2(Cross.Z, Dot);
	float AngleDeg = FMath::RadiansToDegrees(AngleRad);

	if (AngleDeg < 0) AngleDeg += 360.0f;

	int32 Index = FMath::RoundToInt(AngleDeg / 45.0f) % 8;

	return (EDashDirection)Index;
}

void UGA_Dash::PlayMontage()
{
	EDashDirection Dir = GetDashDirectionEnum();

	TObjectPtr<UAnimMontage>* FoundMontage = DashMontage.Find(Dir);
	if (!FoundMontage || !*FoundMontage)
	{
		EndAbility(
			CurrentSpecHandle,
			CurrentActorInfo,
			CurrentActivationInfo,
			true,
			true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		*FoundMontage,
		1.0f
	);

	Task->OnCompleted.AddDynamic(this, &UGA_Dash::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_Dash::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_Dash::OnMontageCancelled);

	Task->ReadyForActivation();
}
