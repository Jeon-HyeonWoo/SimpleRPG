// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "GameFramework/Pawn.h"

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	OwnerPawn = TryGetPawnOwner();
	if (!IsValid(OwnerPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerPawn is invalid : %d, %hs"), __LINE__, __FUNCTION__);
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
}

void UMonsterAnimInstance::UpdateSpeed()
{
	//추후 Z값을 삭제하고 싶다면 Size2D로 변경하여 BlendSpace가 튀지 않도록 하기.
	Speed = OwnerVelocity.Length();
}
