// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHPBarWidget.h"
#include "Components/ProgressBar.h"


void UMonsterHPBarWidget::SetHP(float Current, float Max)
{
	if (MonsterHPBar == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterHPBar is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	if (Max == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Division By Zero : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	float Percent = Current / Max;
	MonsterHPBar->SetPercent(Percent);
}
