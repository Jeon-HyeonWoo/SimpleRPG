// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class SIMPLERPG_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AMonsterAIController();

	virtual void OnPossess(APawn* InPawn) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|AI")
	TObjectPtr<UAISenseConfig_Sight> SenseConfigSight;
};
