// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameplayAbility.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "SimpleRPG/AbilitySystem/Data/DamageDataTableRow.h" 

void UPlayerGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{

	//CoolDown GE 가져오기
	UGameplayEffect* CoolDownGE = GetCooldownGameplayEffect();
	if (!CoolDownGE)
	{
		return;
	}

	//GameplayEffectSepcHandle로 CoolDownGE기반 Spec 생성
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(
		CoolDownGE->GetClass(),
		GetAbilityLevel()
	);

	//Spec 생성 및 유효성 검사
	if (!SpecHandle.IsValid())
	{
		return;
	}

	//SpecHandle 내부 Spec Data 가져오기 및 유효성 검사
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	if (!Spec)
	{
		return;
	}

	//TODO : 추후 Skill 통합 DA로 변경
	const FDamageDataTableRow* Row = CooldownDataRow.GetRow<FDamageDataTableRow>(TEXT("ApplyCooldown"));

	//DT Row 유효성 검사
	if (!Row)
	{
		return;
	}

	//Skill Cooldown 가져오기
	float Duration = Row->CoolDown;

	//SetByCaller로 Tag에 값 주입
	Spec->SetSetByCallerMagnitude(SimpleRPGGameplayTags::SetByCaller_CoolDown_Duration, Duration);
	
	//하위 GA에 등록된 CooldownTags를 ASC에 부여
	Spec->DynamicGrantedTags.AppendTags(CooldownTags);

	//등록
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}
