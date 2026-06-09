// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ResetMonster.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UBTTaskNode_ResetMonster : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTaskNode_ResetMonster();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* Memory) override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

public:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IsOutOfLeashKey;
};
