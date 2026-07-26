// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GenericTeamAgentInterface.h"
#include "SimpleRPG/Teams/SimpleRPGTeamRelationData.h"
#include "SimpleRPG/Teams/SimpleRPGTeam.h"
#include "SimpleRPGTeamSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API USimpleRPGTeamSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	ETeamAttitude::Type GetAttitudeBetween(FGenericTeamId MyId, const AActor& Other) const;

private:

	ETeamAttitude::Type GetAttitude(FGenericTeamId MyId, FGenericTeamId OtherId) const;

private:

	UPROPERTY()
	TObjectPtr<USimpleRPGTeamRelationData> RelationData;
};
