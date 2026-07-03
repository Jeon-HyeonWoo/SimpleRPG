// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGGameplayAbility.h"
#include "GA_SwordAttack.generated.h"

USTRUCT(BlueprintType)
struct FComboStep
{
	GENERATED_BODY()

	//SectionName : Combo1, 2, 3
	UPROPERTY(EditDefaultsOnly)
	FName SectionName;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle DamageRow;
};

/**
* 
 */
UCLASS(Abstract, Blueprintable)
class SIMPLERPG_API UGA_SwordAttack : public USimpleRPGGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_SwordAttack();

	virtual void InputPressed(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo
	) override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

	//Montage Callback
public: 

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

protected:

	UFUNCTION()
	void OnComboWindowOpen(FGameplayEventData PayLoad);

	UFUNCTION()
	void OnComboWindowClose(FGameplayEventData PayLoad);

	UFUNCTION()
	void OnDamageEvent(FGameplayEventData PayLoad);

private:
	//AbilityTask_PlayMontageAndWait
	void PlayComboMontage();

	void DamageEventTask();

	void ResetCombo();
	void ReserveNextCombo();


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TObjectPtr<UAnimMontage> ComboMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TArray<FComboStep> ComboSteps;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	int32 CurrentComboCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	bool bComboWindowOpen = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	bool bNextComboQueued = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	bool bComboReserved = false;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TSubclassOf<UGameplayEffect> BlockMovementEffect;

	FActiveGameplayEffectHandle BlockMovementEffectHandle;

public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|DamageEffect")
	TSubclassOf<UGameplayEffect> DamageEffect;
};

/*
* 힘들었던 점.
* 1. 단일 콤보 Animation or combo별 Animation 3개 결정 -> 복귀모션 부재로 고민 -> 복귀 모션을 포기하고 Idle과 Blending하는 것으로 타협
* 2. JumpToSection 이용시 Animation이 튀는 현상 -> Section 맞추기의 한계 -> SetCurrentNextSection으로 section 예약 
* 3. 예약 방식의 어려움 -> CurrentSection이 끝나는 지점에 WindowClose를 가까이 붙이면 예약 방식이 어려움
* 4. OpenWindow에서 예약을 시작 
* 5. 해결방법
* Input -> Open -> ComboQueue 확인 -> Close Log 순서를 차례로 확인
* Close지점에서 CurrentCombo = None을 찾아냄 

*/

/*
* 1. 데미지 계수 문제
* OnDamageEvent() 에서 ReserveNextCombo에서 CurrentComboCount++가 되면서 ComboCount가 미리 올라가 데미지 계수 문제 밀림 현상
* 
* 2. 원인 
* Count 변수가 다음 Montage 재생과 Damage 부분 두 곳에서 사용됨
* 
*/