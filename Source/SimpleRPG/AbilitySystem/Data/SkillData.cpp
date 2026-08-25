// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillData.h"

const FSkillDataTableRow* USkillData::GetSkillDataTableRow() const
{
	if (!BalancingDataRow.DataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("BalancingDataRow.DataTable is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	const FSkillDataTableRow* FoundRow = 
		BalancingDataRow.DataTable->FindRow<FSkillDataTableRow>(BalancingDataRow.RowName, TEXT("USkillData::GetBalancingRow"));

	if (!FoundRow)
	{
		UE_LOG(LogTemp, Error, TEXT("Row [%s] not found : %d, %hs"), *BalancingDataRow.RowName.ToString(), __LINE__, __FUNCTION__);
		return nullptr;
	}

	return FoundRow;
}

bool USkillData::CanUseWithWeaponTags(const FGameplayTagContainer& InWeaponTags) const
{
	if (RequireWeaponTags.IsEmpty())
	{
		return true;
	}
	
	return InWeaponTags.HasAny(RequireWeaponTags);
}
