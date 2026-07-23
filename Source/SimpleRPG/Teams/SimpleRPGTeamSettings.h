// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SimpleRPG/Teams/SimpleRPGTeamRelationData.h"
#include "SimpleRPGTeamSettings.generated.h"

/**
 * 
 */
//Config = Game : DefaultGame.ini 저장
//DefaultConfig : 프로젝트 전체 공용 설정
//
UCLASS(config = Game, DefaultConfig, meta = (DisplayName = "SimpleRPG Team"))
class SIMPLERPG_API USimpleRPGTeamSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public: 
	//해당 변수를 Ini에 저장
	UPROPERTY(Config, EditAnywhere, Category = "Team")
	TSoftObjectPtr<USimpleRPGTeamRelationData> TeamRelationData;
};
