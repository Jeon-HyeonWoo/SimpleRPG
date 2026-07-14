// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterActionData.h"

const FMonsterMontageEntry* FMonsterActionData::GetRandomEntry() const
{
	if (MontageEntries.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("MontageEntries count = 0 : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}
	
	int32 RandomInt = FMath::RandRange(0, MontageEntries.Num() - 1);

	if (MontageEntries[RandomInt].Montage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Montage is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	return &MontageEntries[RandomInt];
}
