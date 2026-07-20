// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "SimpleRPG/Monster/Data/MonsterActionData.h"
#include "SimpleRPG/Monster/Data/MonsterData.h"
#include "SimpleRPG/Monster/Data/MonsterDamageDataTableRow.h"

ASimpleRPGMonsterBase* UMonsterGameplayAbility::GetOwningMonster() const
{
	
	ASimpleRPGMonsterBase* MonsterCharacter = Cast<ASimpleRPGMonsterBase>(GetAvatarActorFromActorInfo());
	if (!MonsterCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterCharacter invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}
	
	return MonsterCharacter;
}

const FMonsterMontageEntry* UMonsterGameplayAbility::GetRandomMontageEntry(const FGameplayTag& ActionTag) const
{
	ASimpleRPGMonsterBase* Monster = GetOwningMonster();
	if (!Monster)
	{
		return nullptr;
	}

	UMonsterData* MonsterData = GetOwningMonster()->GetMonsterData();
	if (!MonsterData)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterData invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}
	
	const FMonsterActionData* ActionData = MonsterData->ActionDataMap.Find(ActionTag);
	if (!ActionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterActionData invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	return ActionData->GetRandomEntry();
}

bool UMonsterGameplayAbility::PlayActionMontage(const FGameplayTag& ActionTag)
{
	const FMonsterMontageEntry* Entry = GetRandomMontageEntry(ActionTag);
	if (!Entry)
	{
		return false;
	}

	CurrentEntry = Entry;

	UAnimMontage* Montage = Entry->Montage;
	if (!Montage)
	{
		return false;
	}

	if (MontageTask)
	{
		MontageTask->EndTask();
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage,
		1.0f
	);

	Task->OnCompleted.AddDynamic(this, &UMonsterGameplayAbility::HandleMontageCompleted);
	Task->OnCancelled.AddDynamic(this, &UMonsterGameplayAbility::HandleMontageCancelled);

	MontageTask = Task;

	Task->ReadyForActivation();

	return true;
}

bool UMonsterGameplayAbility::GetMonsterSkillRatio(const FDataTableRowHandle& RowHandle, float& OutRatio) const
{
	OutRatio = 0.0f;

	FMonsterDamageDataTableRow* Row = RowHandle.GetRow<FMonsterDamageDataTableRow>(TEXT("GetMonsterSkillRatio"));
	if (!ensureMsgf(Row, TEXT("Row is invalid")))
	{
		return false;
	}

	OutRatio = Row->SkillRatio;

	return true;
}

void UMonsterGameplayAbility::HandleMontageCompleted()
{
	OnActionMontageCompleted();
}

void UMonsterGameplayAbility::HandleMontageCancelled()
{
	OnActionMontageCancelled();
}

void UMonsterGameplayAbility::OnActionMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMonsterGameplayAbility::OnActionMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
