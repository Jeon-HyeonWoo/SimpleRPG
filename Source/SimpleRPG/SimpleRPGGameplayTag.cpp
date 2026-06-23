// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGGameplayTag.h"

namespace SimpleRPGGameplayTags
{
#pragma region State Tag

	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Blocked, "State.Movement.Blocked");

	UE_DEFINE_GAMEPLAY_TAG(State_Dashing, "State.Dasing");

	UE_DEFINE_GAMEPLAY_TAG(State_Acting, "State.Acting");
	UE_DEFINE_GAMEPLAY_TAG(State_Acting_Attacking, "State.Acting.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(State_Acting_Swapping, "State.Acting.Swapping");
	

#pragma endregion
	
#pragma region SetByCaller Tag

	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Skill_Ratio, "SetByCaller.Skill.Ratio");

#pragma endregion

#pragma region Damage Tag

	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_AD, "Damage.Type.AD");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Type_AP, "Damage.Type.AP");

#pragma endregion

}