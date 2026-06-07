// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_InitializeBlackboard.h"
#include "SimpleRPG/Monster/AI/MonsterAIController.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_InitializeBlackboard::UBTTaskNode_InitializeBlackboard()
{
	NodeName = "Initialize BloackBoard";

	//Location Key값 Vector로 고정, 사고 방지
	{
		SpawnLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, SpawnLocationKey));
	}
}

EBTNodeResult::Type UBTTaskNode_InitializeBlackboard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//1. BB 유효성 검사
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BlackboardComponent is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	//2. AIController 유효성 검사
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	//3. AIController's OwnerPawn 유효성 검사
	ASimpleRPGMonsterBase* MonsterPawn = Cast<ASimpleRPGMonsterBase>(AIController->GetPawn());
	if (!IsValid(MonsterPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterBase is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	//4. Vector의 값이 Set이 되어있는가?
	bool bAlreadySet = BBComp->IsVectorValueSet(SpawnLocationKey.SelectedKeyName);

	//5. Vector값 설정이 true라면 Succeeded 반환
	if (bAlreadySet)
	{
		return EBTNodeResult::Succeeded;
	}
	else //아니라면 값을 설정
	{
		BBComp->SetValueAsVector(SpawnLocationKey.SelectedKeyName, MonsterPawn->GetActorLocation());
	}
	


	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_InitializeBlackboard::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	//이 Task를 사용하는 BT로 BB데이터 가져오기
	UBlackboardData* BBData = GetBlackboardAsset();
	if (BBData == nullptr)
	{
		return;
	}

	//Selector를 이 BB에 연결 (DropDown, Type이 동작)
	SpawnLocationKey.ResolveSelectedKey(*BBData);
}
