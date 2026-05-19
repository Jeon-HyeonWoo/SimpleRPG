// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGGameplayAbility.h"
#include "GA_MonsterMeleeAttack.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMonsterAttackMontageData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditDefaultsOnly)
	FName DamageRowName;
};

UCLASS()
class SIMPLERPG_API UGA_MonsterMeleeAttack : public USimpleRPGGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_MonsterMeleeAttack();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* Actorinfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* Actorinfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

private:
	
	//AbilityTask_PlayMontageAndWait 세팅
	void PlayMontage();

	//AbbilityTask_WaitGameplayEvent 세팅
	void WaitForHitEvent();

	//히트 시 데미지 적용
	UFUNCTION()
	void OnHitEventReceived(FGameplayEventData PayLoad);

	//Montage 콜백
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Montage")
	TArray<FMonsterAttackMontageData> AttackMontageDataArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Damage")
	TObjectPtr<UDataTable> DamageDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Damage")
	FGameplayTag HitEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Ability")
	TSubclassOf<UGameplayEffect> BlockMovementEffect;


private:

	FActiveGameplayEffectHandle BlockMovementEffectHandle;

	int32 MontageDataIndex = 0;
};

/* 잠깐 정리용
	
	1. Monster가 Player를 찾아서 공격 범위 안에 들어가면 MeleeAttackTask를 실행
	2. MeleeAttackTask에서 GA_MonsterMeleeAttack 실행
	3. GA_MonsterMeleeAttack에서 PlayMontage 실행
	4. Montage에 설정된 ANS_MeleeHitTrace에서 HitTrace를 발동
	5. HitTrace에 발동된 정보는 SendGameplayEventToActor를 통해 OwnerActor에게 Tag와 함께 Target(Player)정보를 보냄
	6. GA_MonsterMeleeAttack은 SendGameplayEventToActor에서 Tag를 인식하고 PayLoad정보(Target이 포함된)를 가지게 됨.
	7. OnHiyEventReceived에서 DataTable로 정리된 Monster Damage의 배율을 가져옴
	8. DamageEffect의 SpecHandle을 만들어 배율을 넘길 준비
	9. SpecHandle.Data에 SetSetByCaller를 통해 Tag(Key)와 배율(Value)를 저장
	10. ApplyGameplayEffectToTarget을 통해 Target에게 Effect 전송
	11. 전송 과정중에 CalcExec클래스의 Exectuion함수에서 Capture된 Source(Monster)의 Damage값과
	SetSetByCaller를 통해 받은 배율의 값을 계산하여 최종적으로 Target에게 전달
	12. OutExecutionOutput.AddOutputModifier 함수에서 Modifier될 Atrribute와, Attribute 방식, 그리고 최종 값을 전달
*/