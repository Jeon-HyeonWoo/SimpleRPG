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
class UAISenseConfig_Damage;

UCLASS()
class SIMPLERPG_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AMonsterAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
private:
	//Helper Functions

	void SetupSightSense();
	void SetupDamageSense();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|AI")
	TObjectPtr<UAISenseConfig_Sight> SenseConfigSight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|AI")
	TObjectPtr<UAISenseConfig_Damage> SenseConfigDamage;
};
