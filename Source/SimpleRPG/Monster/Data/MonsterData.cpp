// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterData.h"

const FMonsterActionData* UMonsterData::FindActionData(const FGameplayTag& ActionTag) const
{
	if (ActionTag.IsValid() && ActionDataMap.Find(ActionTag))
	{
		return &ActionDataMap[ActionTag];
	}

	return nullptr;
}
