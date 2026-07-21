// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "GameFramework/Pawn.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "SimpleRPG/Monster/Components/MonsterHealthComponent.h"

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	OwnerPawn = TryGetPawnOwner();
	if (!IsValid(OwnerPawn)) return;

	ASimpleRPGMonsterBase* MonsterPawn = Cast<ASimpleRPGMonsterBase>(OwnerPawn);
	if (!MonsterPawn)
	{
		return;
	}

	HealthComponent = MonsterPawn->GetMonsterHealthComponent();
	if (!IsValid(HealthComponent))
	{
		return;
	}
}

void UMonsterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	UpdateSpeed();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(OwnerPawn))
	{
		OwnerVelocity = OwnerPawn->GetVelocity();
	}

	if (IsValid(HealthComponent))
	{
		bIsDead = HealthComponent->IsDead();
	}
}

void UMonsterAnimInstance::UpdateSpeed()
{
	//추후 Z값을 삭제하고 싶다면 Size2D로 변경하여 BlendSpace가 튀지 않도록 하기.
	Speed = OwnerVelocity.Length();
}
