// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SimpleRPG/Teams/SimpleRPGTeam.h"
#include "GenericTeamAgentInterface.h"
#include "SimpleRPGTeamRelationData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSimpleRPGTeamRelation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	TSet<ESimpleRPGTeam> Allies;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	TSet<ESimpleRPGTeam> Hostiles;

};

UCLASS()
class SIMPLERPG_API USimpleRPGTeamRelationData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	TMap<ESimpleRPGTeam, FSimpleRPGTeamRelation> Relations;

	ETeamAttitude::Type GetAttitude(ESimpleRPGTeam MyTeam, ESimpleRPGTeam OtherTeam) const;

};
