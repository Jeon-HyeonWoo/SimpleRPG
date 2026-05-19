// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MonsterAttack.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UBTTaskNode_MonsterAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTaskNode_MonsterAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	UFUNCTION()
	void OnAbilityEnded(const FAbilityEndedData& AbilityEndedData);

public:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|AI")
	FGameplayTag EventTag;

};
