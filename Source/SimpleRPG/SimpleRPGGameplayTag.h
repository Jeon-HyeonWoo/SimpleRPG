// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace SimpleRPGGameplayTags
{
	//State_Movement 움직임 행동
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Blocked);

// 공용 State
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dashing);

//Ability 정체성 태그
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack); //Attack Root Tag
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_Sword);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Dash);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_WeaponSwap);

//Ability CoolDown 태그
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CoolDown_Common_Dash);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CoolDown_Sword_PowerUp);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CoolDown_Sword_ChargeAttack);

//SetByCaller
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Skill_Ratio);
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_CoolDown_Duration);

//SetByCaller_Monster
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Monster_Ratio);

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
	SIMPLERPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Skill_Sword_PowerUp);
}

