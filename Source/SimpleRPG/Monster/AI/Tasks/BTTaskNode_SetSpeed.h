// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SetSpeed.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UBTTaskNode_SetSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTaskNode_SetSpeed();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	UPROPERTY(EditAnywhere, Category = "SimpleRPG|Speed")
	float SpeedValue = 0.0f;
};
