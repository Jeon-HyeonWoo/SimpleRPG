// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGGameplayTag.h"

namespace SimpleRPGGameplayTags
{
	//Ability Á¤Ã¼¼º 
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack,			"Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Dash,			"Ability.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_Sword,	"Ability.Attack.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Ability_WeaponSwap,		"Ability.WeaponSwap");

	//Ability CoolDown
	UE_DEFINE_GAMEPLAY_TAG(CoolDown_Common_Dash,		"CoolDown.Common.Dash");
	UE_DEFINE_GAMEPLAY_TAG(CoolDown_Sword_PowerUp,		"CoolDown.Sword.PowerUp");
	UE_DEFINE_GAMEPLAY_TAG(CoolDown_Sword_ChargeAttack, "CoolDown.Sword.ChargeAttack");

	
	//State_Movement
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Blocked, "State.Movement.Blocked");

	//SetByCaller
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Skill_Ratio,			"SetByCaller.Skill.Ratio");
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_CoolDown_Duration,	"SetByCaller.CoolDonw.Durtaion");

	//SetByCaller_Monster
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Monster_Ratio, "SetByCaller.Monster.Ratio");

	//Damage
	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_AD, "Damage.Type.AD");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_AP, "Damage.Type.AP");


	//Event_Combat Shared
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_HitDetect,			"Event.Combat.HitDetect");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_Damaged,			"Event.Combat.Damaged");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_Death,				"Event.Combat.Death");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_ComboWindow_Open,	"Event.Combat.ComboWindow.Open");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_ComboWindow_Close,	"Event.Combat.ComboWindow.Close");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_AttackTrigger,	"Event.Combat.AttackTrigger");


	//Evnet_Skill_Sword
	UE_DEFINE_GAMEPLAY_TAG(Event_Skill_Sword_PowerUp, "Event.Skill.Sword.PowerUp");

	//Monster GA Trigger Tag
	UE_DEFINE_GAMEPLAY_TAG(Monster_Event_Attack,	"Monster.Event.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Event_Death,		"Monster.Event.Death");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Event_Stagger,	"Monster.Event.Stagger");
	
	//Monster Action Ability Define Tag
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_Melee,	"Monster.Ability.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Attack_Ranged,	"Monster.Ability.Attack.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Death,			"Monster.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(Monster_Ability_Stagger,			"Monster.Ability.Stagger");
}