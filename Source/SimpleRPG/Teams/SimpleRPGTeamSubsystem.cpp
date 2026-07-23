// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGTeamSubsystem.h"
#include "SimpleRPG/Teams/SimpleRPGTeamSettings.h"

void USimpleRPGTeamSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USimpleRPGTeamSettings* Settings = GetDefault<USimpleRPGTeamSettings>();
}

ETeamAttitude::Type USimpleRPGTeamSubsystem::GetAttitude(FGenericTeamId MyId, FGenericTeamId OtherId) const
{
	return ETeamAttitude::Type();
}
