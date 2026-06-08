// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SetSpeed.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "SimpleRPG/Monster/Data/MonsterData.h"

UBTTaskNode_SetSpeed::UBTTaskNode_SetSpeed()
{
	NodeName = TEXT("Set Speed");
}

EBTNodeResult::Type UBTTaskNode_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

#pragma region valid check

	//1. AIController 유효성 검사
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	
	//2. MonsterBase 유효성 검사
	ASimpleRPGMonsterBase* MonsterBase = Cast<ASimpleRPGMonsterBase>(AIController->GetPawn());
	if (!IsValid(MonsterBase))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterBase is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}
	
	//3. MonsterData 가져오기, 유효성 검사
	UMonsterData* MonsterData = MonsterBase->GetMonsterData();
	if (!IsValid(MonsterData))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterData is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}
#pragma endregion

	const FMonsterAIConfig& AIConfig = MonsterData->AIConfig;
	float TargetSpeed = 0.0f;
	
	switch (SpeedType)
	{
	case ESpeedType::Patrol:
		TargetSpeed = AIConfig.PatrolSpeed;
		break;
	case ESpeedType::Chase:
		TargetSpeed = AIConfig.ChaseSpeed;
		break;
	case ESpeedType::Return:
		TargetSpeed = AIConfig.ReturnSpeed;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("UnExpected Value of SpeedType : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	UCharacterMovementComponent* CharacterMovementComponent = MonsterBase->GetCharacterMovement();
	if (!IsValid(CharacterMovementComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterMovementSpeed is invalid: %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}
	CharacterMovementComponent->MaxWalkSpeed = TargetSpeed;

	return EBTNodeResult::Succeeded;
}
