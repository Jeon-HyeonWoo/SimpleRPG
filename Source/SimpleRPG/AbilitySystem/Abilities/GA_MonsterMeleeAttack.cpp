// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MonsterMeleeAttack.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SimpleRPG/AbilitySystem/Data/MonsterDamageDataTableRow.h"


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
	if (BlockMovementEffectHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(BlockMovementEffectHandle);
	}

	Super::EndAbility(Handle, Actorinfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

	//DataTable에서 Row가져오기
	FName RowName = AttackMontageDataArray[MontageDataIndex].DamageRowName;
	FMonsterDamageDataTableRow* Row = DamageDataTable->FindRow<FMonsterDamageDataTableRow>(RowName, TEXT(""));
	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is nullptr : %d, %hs"), *RowName.ToString(), __LINE__, __FUNCTION__);
		return;
	}

	float Ratio = FMath::RandRange(Row->MinMultiplier, Row->MaxMultiplier);

	//SpecHandle 만들기
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect, GetAbilityLevel());

	SpecHandle.Data.Get()->SetSetByCallerMagnitude(FName("Data.Damage.Multiplier"), Ratio);

	//Target에 SpecHandle Data값 적용(MonsterCalcExec가 최종적으로 계산하여 Target의 AttributeSet 값 조절)
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(),
		TargetASC
	);
}

void UGA_MonsterMeleeAttack::OnMontageCompleted()
{
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false
	);
}

void UGA_MonsterMeleeAttack::OnMontageCancelled()
{
	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		true
	);
}
