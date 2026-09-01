// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_SwordAttack.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../Data/DamageDataTableRow.h"
#include "SimpleRPG/AbilitySystem/Data/SkillData.h"
#include "SimpleRPG/AbilitySystem/Data/SkillDataTableRow.h"
#include "SimpleRPG/Monster/AbilitySystem/AttributeSet/SimpleRPGMonsterAttributeSet.h"

UGA_SwordAttack::UGA_SwordAttack()
{
	/*
	*	기본 인스턴스 정책 = 인스턴스를 만들지 않음
		InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
		이유 : 메모리 절약
		예시 : 패시브 스킬 처럼 멤버 변수가 필요 없는 Ability는 인스턴스를 만들 이유가 없기 때문

		우리가 쓰는 instancedPerActor는 콤보 상태 추적을 위해 멤버 변수 설치 및
		같은 어빌리티를 공유하는 타 액터들과 상태를 달리해야하기 때문

		NonInstanced = SharedPtr, InstancePerActor = UniquePtr 같은 느낌?
	*/
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = false;

	//Ability 정체성
	AbilityTags.AddTag(SimpleRPGGameplayTags::Ability_Attack_Sword);
	
	//활성 중 차단
	BlockAbilitiesWithTag.AddTag(SimpleRPGGameplayTags::Ability_Attack);
	BlockAbilitiesWithTag.AddTag(SimpleRPGGameplayTags::Ability_WeaponSwap);
	
}

void UGA_SwordAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (bComboWindowOpen)
	{
		ReserveNextCombo();
	}
	else
	{
		bNextComboQueued = true;
	}
}

void UGA_SwordAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	/*
	* SpecHandle : Spec 포인터라고 생각, ClearAbility 할 때 Handle번호를 받아서 제거
	* ActorInfo : Ability를 소유한 Actor 정보, ActorInfo->AvatarActor로 Character에 접근, ActorInfo->ASC로 ASC에 접근 등등
	* ActivationInfo : 어떻게 활성화가 되었는가?
	* TriggerEventData : Event로 Trigger된 경우, 우리는 input으로 하기에 Nullptr
	*/

	/* CommitAbility : Ability실행 최종 확인 함수 */
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ApplyBlockMovement();

	ResetCombo();
	CurrentComboCount = 1;

	PlayComboMontage();

	UAbilityTask_WaitGameplayEvent* WaitOpenTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,		//소유 Ability
		SimpleRPGGameplayTags::Event_Combat_ComboWindow_Open // 기다릴 태그
	);
	

	/* Event 도착시 호출될 callback Function */
	WaitOpenTask->EventReceived.AddDynamic(this, &UGA_SwordAttack::OnComboWindowOpen);	
	/* 실행 함수 */
	WaitOpenTask->ReadyForActivation();		

	UAbilityTask_WaitGameplayEvent* WaitCloseTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		SimpleRPGGameplayTags::Event_Combat_ComboWindow_Close
	);
	WaitCloseTask->EventReceived.AddDynamic(
		this,
		&UGA_SwordAttack::OnComboWindowClose
	);
	WaitCloseTask->ReadyForActivation();

	DamageEventTask();
}

void UGA_SwordAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	/*
	* bReplicateEndAbility : 멀티플레이어에서 종료를 네트워크로 전파할지 여부
	* bWasCancelled : 정상 종료인지, Cancel인지 구분하는 Flag, OnMotageCompleted에서 호출하면 False, OnMontageCancelled = true
	*/
	ResetCombo();
	RemoveBlockMovement();
	/*
	* EndAbility 기능 : 
	* AbilityState = 비활성
	* GameplayTag 해제,
	* Activate AbilityTask 정리,
	* ASC에 Ability가 끝났다를 알림
	*/
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UGA_SwordAttack::OnMontageCompleted()
{
	/* 정상 종료, 콤보 입력 없이 몽타주가 끝까지 재생되었거나, 콤보 피니시 후 종료 */
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_SwordAttack::OnMontageCancelled()
{	
	UE_LOG(LogTemp, Warning, TEXT(">> OnMontageCancelled"));
	/* 비정상 종료, 피격 및 다른 Ability나 행동으로 의해 끊길 경우 */
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_SwordAttack::OnComboWindowOpen(FGameplayEventData PayLoad)
{
	bComboWindowOpen = true;
	bComboReserved = false;

	if (bNextComboQueued)
	{
		ReserveNextCombo();
		bNextComboQueued = false;
	}
	
}

void UGA_SwordAttack::OnComboWindowClose(FGameplayEventData PayLoad)
{
	bComboWindowOpen = false;
}

void UGA_SwordAttack::OnDamageEvent(FGameplayEventData PayLoad)
{
	//현재 재생중인 Montage Secton Name을 가져옴
	FName CurrentSection = GetAbilitySystemComponentFromActorInfo()->GetCurrentMontageSectionName();

	const USkillData* SkillData = Cast<USkillData>(GetCurrentSourceObject());
	if (!SkillData)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillData is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	const FSkillDataTableRow* Row = SkillData->GetSkillDataTableRow();
	if (!Row)
	{
		return;
	}

	//TODO : FSkillProgressState 도입 시 실제 CurrentGrade를 전달
	const float Ratio = Row->GetDamageMultifilerAtGrade(CurrentSection, 1);
	if (Ratio <= 0.0f)
	{
		//해당 단계를 찾지 못한 경우
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Stage : %s / Ratio : %.2f"), *CurrentSection.ToString(), Ratio);

	AActor* Target = const_cast<AActor*>(PayLoad.Target.Get());
	if (!IsValid(Target))
	{
		UE_LOG(LogTemp, Error, TEXT("Target is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}


	ApplyDamageToTarget(Target, DamageEffect, Ratio);
}


void UGA_SwordAttack::PlayComboMontage()
{
	//1. Montage null 체크
	if (!ComboMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	//2. Task Create
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		ComboMontage,
		1.0f,
		ComboSteps[0].SectionName
	);

	//3. Binding 
	Task->OnCompleted.AddDynamic(this, &UGA_SwordAttack::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_SwordAttack::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_SwordAttack::OnMontageCancelled);

	//4. Flush
	Task->ReadyForActivation();
}

void UGA_SwordAttack::DamageEventTask()
{
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		SimpleRPGGameplayTags::Event_Combat_HitDetect
	);
	
	Task->EventReceived.AddDynamic(this, &UGA_SwordAttack::OnDamageEvent);

	Task->ReadyForActivation();
}

void UGA_SwordAttack::ResetCombo()
{
	CurrentComboCount = 0;
	bComboWindowOpen = false;
	bNextComboQueued = false;
	bComboReserved = false;
}

void UGA_SwordAttack::ReserveNextCombo()
{
	if (!ComboSteps.IsValidIndex(CurrentComboCount)) return;

	FName CurrentSection = ComboSteps[CurrentComboCount - 1].SectionName;
	FName NextSection = ComboSteps[CurrentComboCount].SectionName;

	GetAbilitySystemComponentFromActorInfo()
		->CurrentMontageSetNextSectionName(CurrentSection, NextSection);

	CurrentComboCount++;
	bComboReserved = true;
}
