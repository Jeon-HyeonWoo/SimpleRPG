// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataTableRow.h"

float FSkillDataTableRow::GetDamageMultifilerAtGrade(int32 InGrade) const
{	
	const int32 SafeGrade = GetClampedGrade(InGrade);

	return DamageMultiplier + ((SafeGrade - 1) * DamageMultiplierPerGrade);
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


