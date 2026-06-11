// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SetInvulnerable.h"
#include "AIController.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"

UBTTaskNode_SetInvulnerable::UBTTaskNode_SetInvulnerable()
{
	NodeName = "Set Invulnerable";
}

EBTNodeResult::Type UBTTaskNode_SetInvulnerable::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
#pragma region valid check

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
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

	MonsterPawn->ApplyInvulnerability();

	return EBTNodeResult::Succeeded;
}
