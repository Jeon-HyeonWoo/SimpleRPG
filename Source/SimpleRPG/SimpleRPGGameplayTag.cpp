// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGGameplayTag.h"

namespace SimpleRPGGameplayTags
{
	//State
	UE_DEFINE_GAMEPLAY_TAG(State_Dashing, "State.Dasing");

	//Ability Á¤Ã¼¼º 
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack, "Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Dash, "Ability.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_Sword, "Ability.Attack.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Ability_WeaponSwap, "Ability.WeaponSwap");

	//State_Movement
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Blocked, "State.Movement.Blocked");

	//State_Acting
	UE_DEFINE_GAMEPLAY_TAG(State_Acting, "State.Acting");
	UE_DEFINE_GAMEPLAY_TAG(State_Acting_Attacking, "State.Acting.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(State_Acting_Swapping, "State.Acting.Swapping");
	

	//SetByCaller
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Skill_Ratio, "SetByCaller.Skill.Ratio");

	//Damage
	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_AD, "Damage.Type.AD");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_AP, "Damage.Type.AP");


	//Event_Combat Shared
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_HitDetect, "Event.Combat.HitDetect");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_Damaged, "Event.Combat.Damaged");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_Death, "Event.Combat.Death");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_ComboWindow_Open, "Event.Combat.ComboWindow.Open");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_ComboWindow_Close, "Event.Combat.ComboWindow.Close");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combat_AttackTrigger, "Event.Combat.AttackTrigger");


	//Evnet_Skill_Sword
	UE_DEFINE_GAMEPLAY_TAG(Event_Skill_Sword_PowerUp, "Event.Skill.Sword.PowerUp");


}