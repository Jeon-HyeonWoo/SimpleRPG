// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace SimpleRPGGameplayTags
{
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Blocked);

	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Acting);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Acting_Attacking);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Acting_Swapping);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dashing);


	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Skill_Ratio);

	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Type_AD);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Type_AP);
}