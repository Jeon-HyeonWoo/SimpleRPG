// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGTeamRelationData.h"

ETeamAttitude::Type USimpleRPGTeamRelationData::GetAttitude(ESimpleRPGTeam MyTeam, ESimpleRPGTeam OtherTeam) const
{
	//Value 값을 반환 받음
	const FSimpleRPGTeamRelation* MyRow = Relations.Find(MyTeam);
	
	
	if (!MyRow) return ETeamAttitude::Neutral;

	if (MyRow->Hostiles.Contains(OtherTeam)) return ETeamAttitude::Hostile;
	
	if (MyRow->Allies.Contains(OtherTeam)) return ETeamAttitude::Friendly;

	return ETeamAttitude::Neutral;
}
