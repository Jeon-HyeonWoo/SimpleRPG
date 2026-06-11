// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SetInvulnerable.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UBTTaskNode_SetInvulnerable : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTaskNode_SetInvulnerable();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
