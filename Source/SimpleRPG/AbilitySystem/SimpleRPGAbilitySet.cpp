// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "Abilities/SimpleRPGGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "Data/SkillData.h"

void FSimpleRPGAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FSimpleRPGAbilitySet_GrantedHandles::RemoveFromAbilitySystem(UAbilitySystemComponent* ASC)
{
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, AbilitySystemComponent is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			ASC->ClearAbility(Handle);
		}
	}
	AbilitySpecHandles.Reset();
}

void USimpleRPGAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* ASC, FSimpleRPGAbilitySet_GrantedHandles* OutGrantedHandles) const
{
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, AbilitySystemComponent is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	for (const FSimpleRPGAbilitySetEntry& Entry : GrantedAbilities)
	{
		TSubclassOf<USimpleRPGGameplayAbility> ClassToGrant = Entry.AbilityClass;

		if (IsValid(Entry.SkillData))
		{
			ClassToGrant = Entry.SkillData->LogicAbilityClass;
		}

		if (!IsValid(ClassToGrant))
		{
			UE_LOG(LogTemp, Warning, TEXT("AbilityClass is invalid in AbilitySet [%s] : %d, %hs"),
				*GetNameSafe(this), __LINE__, __FUNCTION__);
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("Grant: %s / SkillData: %s"),
			*GetNameSafe(ClassToGrant), *GetNameSafe(Entry.SkillData));

		/* Spec = 말 그대로 Ability에 대한 스펙, 정보 묶음 AbilityClass와 Level, 부여오브젝트, 런타임테그, 핸들(포인터개념)을 포함 */
		FGameplayAbilitySpec AbilitySpec(Entry.AbilityClass, 1, static_cast<int32>(Entry.InputID), Entry.SkillData);

		/* GiveAbility의 반환값이 SpecHandle 해당 Handle번호를 변수로 만들어서 Granted에 부여 */
		FGameplayAbilitySpecHandle GrantedHandle = ASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(GrantedHandle);
		}
	}

}


