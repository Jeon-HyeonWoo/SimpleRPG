// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SetSpeed.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

UBTTaskNode_SetSpeed::UBTTaskNode_SetSpeed()
{
	NodeName = TEXT("Set Speed");
}

EBTNodeResult::Type UBTTaskNode_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	//Check MonsterBase
	ASimpleRPGMonsterBase* MonsterBase = Cast<ASimpleRPGMonsterBase>(AIController->GetPawn());
	if (!MonsterBase)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterBase is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	MonsterBase->GetCharacterMovement()->MaxWalkSpeed = SpeedValue;

	return EBTNodeResult::Succeeded;
}
