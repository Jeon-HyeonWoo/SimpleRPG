// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataTableRow.h"

const FSkillStageBalance* FSkillDataTableRow::FindStage(FName InStageId) const
{
	return Stages.FindByPredicate(
		[InStageId](const FSkillStageBalance& InStage)
		{
			return InStage.StageId == InStageId;
		}
	);
}

float FSkillDataTableRow::GetDamageMultifilerAtGrade(FName InStageId, int32 InGrade) const
{
	const FSkillStageBalance* Stage = FindStage(InStageId);
	if (!Stage)
	{
		UE_LOG(LogTemp, Error, TEXT("Stage [%s] not Found : %d, %hs"), *InStageId.ToString(), __LINE__, __FUNCTION__);
		return 0.0f;
	}

	const int32 SafeGrade = GetClampedGrade(InGrade);

	return Stage->DamageMultiplier + ((SafeGrade - 1) * Stage->DamageMultiplierPerGrade);
}

float FSkillDataTableRow::GetCostAtGrade(int32 InGrade) const
{
	const int32 SafeGrade = GetClampedGrade(InGrade);

	return BaseCostValue + ((SafeGrade - 1) * CostPerGrade);
}

int FSkillDataTableRow::GetClampedGrade(int32 InGrade) const
{
	const int32 SafeMaxGrade = FMath::Max(1, MaxGrade);

	return FMath::Clamp(InGrade, 1, SafeMaxGrade);
}


