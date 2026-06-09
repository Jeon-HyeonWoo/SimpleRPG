// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ResetMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
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

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BlackboardComponent is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

#pragma endregion

	//Recover full Hp
	MonsterPawn->RestoreHP();

	//IsOutOfLeashKey 해제
	BBComp->SetValueAsBool(IsOutOfLeashKey.SelectedKeyName, false);

	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_ResetMonster::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	//이 Task를 사용하는 BT로 BB데이터 가져오기
	UBlackboardData* BBData = GetBlackboardAsset();
	if (BBData == nullptr)
	{
		return;
	}

	//Selector를 이 BB에 연결 (DropDown, Type이 동작)
	IsOutOfLeashKey.ResolveSelectedKey(*BBData);
	
}
