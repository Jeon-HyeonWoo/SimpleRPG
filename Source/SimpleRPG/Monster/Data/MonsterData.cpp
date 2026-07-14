// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterData.h"

const FMonsterActionData* UMonsterData::FindActionData(const FGameplayTag& ActionTag) const
{
    if (!ActionTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("ActionTag invalid : %d, %hs"), __LINE__, __FUNCTION__); 
        return nullptr;
    }

    const FMonsterActionData* Found = ActionDataMap.Find(ActionTag);
    if (!Found)
    {
        // 로그 (태그명 + DA명 포함)
        UE_LOG(LogTemp, Warning, TEXT("ActionData invalid, Tag : %s, DA : %s : %d, %hs"), *ActionTag.ToString(), *GetName(), __LINE__, __FUNCTION__);
        return nullptr;
    }

    return Found;
}
