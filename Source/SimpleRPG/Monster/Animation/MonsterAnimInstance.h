// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

/**
 * 
 */
class APawn;
class UMonsterHealthComponent;

UCLASS()
class SIMPLERPG_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	//override
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	//Helper Function

	void UpdateSpeed();

protected:

	UPROPERTY()
	FVector OwnerVelocity;

	UPROPERTY()
	TObjectPtr<APawn> OwnerPawn;

	UPROPERTY()
	TObjectPtr<UMonsterHealthComponent> HealthComponent;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion", meta=(AllowPrivateAccess="true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool bIsDead = false;
	
};
