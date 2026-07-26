// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGTeamSubsystem.h"
#include "SimpleRPG/Teams/SimpleRPGTeamSettings.h"

void USimpleRPGTeamSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USimpleRPGTeamSettings* Settings = GetDefault<USimpleRPGTeamSettings>();
	if (!Settings)
	{
		UE_LOG(LogTemp, Error, TEXT("TeamSettings invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	if (Settings->TeamRelationData.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("TeamRelationData invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	RelationData = Settings->TeamRelationData.LoadSynchronous();

	if (!RelationData)
	{
		UE_LOG(LogTemp, Error, TEXT("RelationData Load Failed : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}
}

ETeamAttitude::Type USimpleRPGTeamSubsystem::GetAttitude(FGenericTeamId MyId, FGenericTeamId OtherId) const
{
	if (!RelationData)
	{
		UE_LOG(LogTemp, Error, TEXT("RelationData invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return ETeamAttitude::Neutral;
	}

	ESimpleRPGTeam MyTeam = static_cast<ESimpleRPGTeam>(MyId.GetId());
	ESimpleRPGTeam OtherTeam = static_cast<ESimpleRPGTeam>(OtherId.GetId());

 	return RelationData->GetAttitude(MyTeam, OtherTeam);
	
}

ETeamAttitude::Type USimpleRPGTeamSubsystem::GetAttitudeBetween(FGenericTeamId MyId, const AActor& Other) const
{
	const IGenericTeamAgentInterface* TeamAgent = nullptr;

	const APawn* OtherPawn = Cast<APawn>(&Other);
	
	if (OtherPawn && OtherPawn->GetController())
	{
		TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	}

	if (!TeamAgent)
	{
		TeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
	}

	if (!TeamAgent)
	{
		return ETeamAttitude::Neutral;
	}
	

	return GetAttitude(MyId, TeamAgent->GetGenericTeamId());
}
