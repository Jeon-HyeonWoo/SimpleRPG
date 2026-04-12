// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_DashMove.generated.h"

/**
 * 1. Activate() Task가 시작할 때 한 번 호출
 * 2. TickTask(float DeltaTime) 매 프레임 호출
 * 3. OnDestroy() 정리용
 */

/*
* 정해진 방향으로, 정해진 거리를, 정해진 시간동안 이동
* 이동이 끝나면 GA callback 필요
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashMoveDelegate);

UCLASS()
class SIMPLERPG_API UAT_DashMove : public UAbilityTask
{
	GENERATED_BODY()
	
public:

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

public:

	/* Task를 만드는 Factory Function */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Dash Move", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAT_DashMove* CreateDashMove(UGameplayAbility* OwningAbility, FVector Direction, float Distance, float Duration);

public:

	/* BlueprintAssignable : Blueprint에서도 delegate를 바인딩 할 수 있게 해주는 속성 */
	UPROPERTY(BlueprintAssignable)
	FDashMoveDelegate OnCompleted;

private:

	FVector DashDirection;		//Get Factory Fn
	float DashSpeed;			//Distance / Duration
	float DashDuration;			//Total Time
	float ElapsedTime;			//Reach for Duration

	bool bOriginalOrientRotationToMovement;	//Back up
};
