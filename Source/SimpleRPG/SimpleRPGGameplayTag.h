// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace SimpleRPGGameplayTags
{

// 공용 State
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dashing);



//State_Movement 움직임 행동
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Blocked);

//State_Acting 취하고 있는 행동
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Acting);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Acting_Attacking);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Acting_Swapping);

//SetByCaller 
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Skill_Ratio);


//Damage 데미지 관련
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Type_AD);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Type_AP);

//Event_Combat 공용 전투
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_HitDetect);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_Damaged);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_Death);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_ComboWindow_Open);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_ComboWindow_Close);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_AttackTrigger);

//Event_Skill 액티브 스킬 발동
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Evnet_Skill_Sword_PowerUp);
}

