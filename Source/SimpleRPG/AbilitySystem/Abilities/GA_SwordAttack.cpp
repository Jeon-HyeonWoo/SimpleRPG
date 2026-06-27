// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_SwordAttack.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../Data/DamageDataTableRow.h"
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
	
	// 활성화 도중 재 활성화 방지
	FGameplayTag SwordAttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.SwordAttack"));

	//이 Ability가 활성화되면 캐릭터에 Tag부여
	ActivationOwnedTags.AddTag(SwordAttackTag);

	// 캐릭터에 이 Tag가 붙어있으면 Ability활성화 차단
	ActivationBlockedTags.AddTag(SwordAttackTag);


	//Acting 계열 상호 차단
	ActivationOwnedTags.AddTag(SimpleRPGGameplayTags::State_Acting_Attacking);
	ActivationBlockedTags.AddTag(SimpleRPGGameplayTags::State_Acting);
	ActivationBlockedTags.AddTag(SimpleRPGGameplayTags::State_Dashing);
}

void UGA_SwordAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//Window 상태를 안 따지고 무조건 저장
	UE_LOG(LogTemp, Warning, TEXT(">>> InputPressed"));
	bNextComboQueued = true;
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

	ResetCombo();
	CurrentComboCount = 1;

	if (BlockMovementEffect)
	{
		BlockMovementEffectHandle = ApplyGameplayEffectToOwner(
			Handle, ActorInfo, ActivationInfo, BlockMovementEffect.GetDefaultObject(), 1.0f
		);
	}

	PlayComboMontage(0);

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
	UE_LOG(LogTemp, Warning, TEXT("GA_SwordAttack::EndAbility - WasCancelled : %s"), bWasCancelled ? TEXT("true") : TEXT("false"));

	if (BlockMovementEffectHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(BlockMovementEffectHandle);
	}

	/*
	* bReplicateEndAbility : 멀티플레이어에서 종료를 네트워크로 전파할지 여부
	* bWasCancelled : 정상 종료인지, Cancel인지 구분하는 Flag, OnMotageCompleted에서 호출하면 False, OnMontageCancelled = true
	*/
	ResetCombo();

	/*
	* EndAbility 기능 : 
	* AbilityState = 비활성
	* GameplayTag 해제,
	* Activate AbilityTask 정리,
	* ASC에 Ability가 끝났다를 알림
	*/
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_SwordAttack::OpenComboWindow()
{
	bComboWindowOpen = true;
}

void UGA_SwordAttack::CloseComboWindow()
{
	bComboWindowOpen = false;
	UE_LOG(LogTemp, Warning, TEXT("CloseWindow : Queued : %d, Count = %d"), bNextComboQueued, CurrentComboCount);
	if (bNextComboQueued)
	{
		StartNextCombo();
	}
}

void UGA_SwordAttack::OnMontageCompleted()
{
	/* 정상 종료, 콤보 입력 없이 몽타주가 끝까지 재생되었거나, 콤보 피니시 후 종료 */
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_SwordAttack::OnMontageCancelled()
{
	UE_LOG(LogTemp, Warning, TEXT("<<< OnMontageCancelled: ..."));
	/* 콤보 전환 시 무시 */
	if (bIsTransitioningCombo)
	{
		return;
	}
	/* 비정상 종료, 피격 및 다른 Ability나 행동으로 의해 끊길 경우 */
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_SwordAttack::OnComboWindowOpen(FGameplayEventData PayLoad)
{
	UE_LOG(LogTemp, Warning, TEXT("ComboWindow Opened"));
	OpenComboWindow(); 
}

void UGA_SwordAttack::OnComboWindowClose(FGameplayEventData PayLoad)
{
	UE_LOG(LogTemp, Warning, TEXT("ComboWindow Close"));
	CloseComboWindow();
}

void UGA_SwordAttack::OnDamageEvent(FGameplayEventData PayLoad)
{
	FName RowName = FName(*FString::Printf(TEXT("SwordAttack_Combo%d"), CurrentComboCount));

	float Ratio;

	if (!GetSkillRatio(DamageDataTable, RowName, Ratio)) return;

	ApplyDamageToTarget(const_cast<AActor*>(PayLoad.Target.Get()), DamageEffect, Ratio);
}

void UGA_SwordAttack::PlayComboMontage(int32 MontageIndex)
{
	/* ComboMontages의 IndexValid check */
	if (!ComboMontages.IsValidIndex(MontageIndex))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	/* 일반 몽타주를 안쓰는 이유 : Ability와 생명주기를 같이 함 + Montage의 기능을 ability와 합쳐서 간소화 */
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,							/* Task를 소유하고 있는 Ability */
		NAME_None,						/* Task Instance 이름, Debug용도 */
		ComboMontages[MontageIndex],	/* 재생할 Montage */
		1.0f,							/* 재생 속도 */
		NAME_None						/* 시작 섹션 이름 -> 단일 Montage라 필요없음 */
	);

	/* Montage의 정상적 재생 종료 */
	MontageTask->OnCompleted.AddDynamic(this, &UGA_SwordAttack::OnMontageCompleted);

	/* 다른 Montage에 의해 끊길 시 ex)피격시 피격 Montage 재생 */
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_SwordAttack::OnMontageCancelled);

	/* Ability 자체가 Cancel될 때, 태그 충돌 및 강제 캔슬 시 호출 */
	MontageTask->OnCancelled.AddDynamic(this, &UGA_SwordAttack::OnMontageCancelled);

	/* Task 실행 */
	MontageTask->ReadyForActivation();
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

void UGA_SwordAttack::StartNextCombo()
{
	bNextComboQueued = false;
	CurrentComboCount++;
	UE_LOG(LogTemp, Warning, TEXT("StartNextCombo: Count=%d, MontageNum=%d"), CurrentComboCount, ComboMontages.Num());

	if (CurrentComboCount > ComboMontages.Num())
	{
		return;
	}

	bIsTransitioningCombo = true;
	PlayComboMontage(CurrentComboCount - 1);
	bIsTransitioningCombo = false;

}

void UGA_SwordAttack::ResetCombo()
{
	CurrentComboCount = 0;
	bComboWindowOpen = false;
	bNextComboQueued = false;
	bIsTransitioningCombo = false;
}
