// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGGameplayTag.h"

namespace SimpleRPGGameplayTags
{
	//Ability 정체성 
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack,			"Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Dash,			"Ability.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_Sword,	"Ability.Attack.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Ability_WeaponSwap,		"Ability.WeaponSwap");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Stagger,			"Ability.Stagger");
	UE_DEFINE_GAMEPLAY_TAG(Ability_KnockDown,		"Ability.KnockDown");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Death,			"Ability.Death");


	//Ability CoolDown
	UE_DEFINE_GAMEPLAY_TAG(CoolDown_Common_Dash,		"CoolDown.Common.Dash");
	UE_DEFINE_GAMEPLAY_TAG(CoolDown_Sword_PowerUp,		"CoolDown.Sword.PowerUp");
	UE_DEFINE_GAMEPLAY_TAG(CoolDown_Sword_ChargeAttack, "CoolDown.Sword.ChargeAttack");

	
	//State_Movement
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Blocked, "State.Movement.Blocked");

	//피격상태 태그
	UE_DEFINE_GAMEPLAY_TAG(HitReact_Stagger,	"HitReact.Stagger");
	UE_DEFINE_GAMEPLAY_TAG(HitReact_KnockDown,	"HitReact.KnockDown");
	UE_DEFINE_GAMEPLAY_TAG(HitReact_None,		"HitReact.None");

	//SetByCaller
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Skill_Ratio,			"SetByCaller.Skill.Ratio");
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_CoolDown_Duration,	"SetByCaller.CoolDonw.Durtaion");


	//Event_Combat Shared
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_HitDetect,			"Event.Combat.HitDetect");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_Damaged,			"Event.Combat.Damaged");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_Death,				"Event.Combat.Death");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_ComboWindow_Open,	"Event.Combat.ComboWindow.Open");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_ComboWindow_Close,	"Event.Combat.ComboWindow.Close");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_AttackTrigger,		"Event.Combat.AttackTrigger");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_Stagger,			"Event.Combat.Stagger");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_KnockDown,			"Event.Combat.KnockDown");

	//Evnet_Skill_Sword
	UE_DEFINE_GAMEPLAY_TAG(Event_Skill_Sword_PowerUp, "Event.Skill.Sword.PowerUp");

	//Monster GA Trigger Tag
	UE_DEFINE_GAMEPLAY_TAG(Monster_Event_Attack,	"Monster.Event.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Event_Death,		"Monster.Event.Death");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Event_Stagger,	"Monster.Event.Stagger");
	
	//Monster Action Ability Define Tag
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack,			"Monster.Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_Melee,	"Monster.Ability.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_Ranged,	"Monster.Ability.Attack.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Death,			"Monster.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Stagger,			"Monster.Ability.Stagger");

	//Monster State
	UE_DEFINE_GAMEPLAY_TAG(Monster_State_Stagger, "Monster.State.Stagger");

	//Player_Skill_Data
	UE_DEFINE_GAMEPLAY_TAG(Skill_Cost_HP, "Skill.Cost.HP");
	UE_DEFINE_GAMEPLAY_TAG(Skill_Cost_MP, "Skill.Cost.MP");

	UE_DEFINE_GAMEPLAY_TAG(Skill_Category_Active, "Skill.Category.Active");
	UE_DEFINE_GAMEPLAY_TAG(Skill_Category_Passive, "Skill.Category.Passive");

	//Damage_Type
	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_AD_Melee, "Damage.Type.AD.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_AD_Range, "Damage.Type.AD.Range");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_AP, "Damage.Type.AP");
	
	//Weapon_Type
	UE_DEFINE_GAMEPLAY_TAG(Weapon_Type_Sword, "Weapon.Type.Sword");


}