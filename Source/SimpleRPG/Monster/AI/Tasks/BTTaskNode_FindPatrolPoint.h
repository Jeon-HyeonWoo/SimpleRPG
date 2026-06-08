// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FindPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UBTTaskNode_FindPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTaskNode_FindPatrolPoint();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

public:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SpawnLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PatrolPointKey;

private:
	//Helper Functions 

	bool FindReachablePoint(const FVector& Origin, const FVector& CurrentLocation, float Range, FVector& OutPoint) const;
};
