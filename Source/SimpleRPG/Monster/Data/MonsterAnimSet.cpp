// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimSet.h"

UAnimMontage* FMonsterAnimSet::GetAnimMontageByTag(FGameplayTag Tag) const
{
	//1. Tag valid check
	if (!Tag.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("GameplayTag is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	const FMonsterAnimList* FoundList = AnimSet.Find(Tag);
	if (!FoundList)
	{
		UE_LOG(LogTemp, Error, TEXT("FoundList is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	UAnimMontage* Montage = nullptr;

	if (FoundList->AnimList.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Animlist is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	int32 Rand = FMath::RandRange(0, FoundList->AnimList.Num() - 1);
	
	Montage = FoundList->AnimList[Rand];

	if (Montage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Montage is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}
	
	return Montage;
}
