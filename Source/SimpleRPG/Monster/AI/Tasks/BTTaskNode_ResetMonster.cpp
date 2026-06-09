// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ResetMonster.h"
#include "SimpleRPG/Monster/AI/MonsterAIController.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"

UBTTaskNode_ResetMonster::UBTTaskNode_ResetMonster()
{
	NodeName = "Monster Reset";
}

EBTNodeResult::Type UBTTaskNode_ResetMonster::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* Memory)
{

#pragma region valid check
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	ASimpleRPGMonsterBase* MonsterPawn = Cast<ASimpleRPGMonsterBase>(AIController->GetPawn());
	if (!IsValid(MonsterPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterPawn is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}
#pragma endregion

	//Recover full Hp
	MonsterPawn->RestoreHP();


	return EBTNodeResult::Succeeded;
}
