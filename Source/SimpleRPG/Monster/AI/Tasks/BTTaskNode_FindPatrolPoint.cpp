// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_FindPatrolPoint.h"
#include "SimpleRPG/Monster/AI/MonsterAIController.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "SimpleRPG/Monster/Data/MonsterAIConfig.h"
#include "SimpleRPG/Monster/Data/MonsterData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTaskNode_FindPatrolPoint::UBTTaskNode_FindPatrolPoint()
{
	NodeName = "Find Patrol Point";
}

EBTNodeResult::Type UBTTaskNode_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//1. BlackboardComponent 유효성 검사
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BlackboardComponent is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	//2. AIController 유효성 검사

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	//3. MonsterPawn 유효성 검사
	ASimpleRPGMonsterBase* MonsterPawn = Cast<ASimpleRPGMonsterBase>(Controller->GetPawn());
	if (!IsValid(MonsterPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterPawn is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	//4. MonsterData 유효성 검사
	UMonsterData* MonsterData = MonsterPawn->GetMonsterData();
	if (!MonsterData)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterData is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	//5. Patrol Range 범위 체크
	float PatrolRange = MonsterData->AIConfig.PatrolRange;
	if (PatrolRange <= 0.0f || PatrolRange > MonsterData->AIConfig.LeashRange)
	{
		UE_LOG(LogTemp, Error, TEXT("PatrolRange is not set or OutOfRange PatrolRange,  : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}
	
	//6. SpawpnLocation, CurrentLoaction 변수 가져오기
	const FVector Origin = BBComp->GetValueAsVector(SpawnLocationKey.SelectedKeyName);
	const FVector CurrentLocation = MonsterPawn->GetActorLocation();
	FVector OutPoint;
	//7. RandomPoint Helper 함수
	bool bFindReachablePoint = FindReachablePoint(Origin, CurrentLocation, PatrolRange, OutPoint);

	if (!bFindReachablePoint)
	{
		UE_LOG(LogTemp, Error, TEXT("bFindReachablePoint is failed : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	BBComp->SetValueAsVector(PatrolPointKey.SelectedKeyName, OutPoint);

	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_FindPatrolPoint::InitializeFromAsset(UBehaviorTree& Asset)
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
	PatrolPointKey.ResolveSelectedKey(*BBData);
}

bool UBTTaskNode_FindPatrolPoint::FindReachablePoint(const FVector& Origin, const FVector& CurrentLocation, float Range, FVector& OutPoint) const
{
	//1.NavigationSystem 유효성 검사
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!IsValid(NavSystem))
	{
		UE_LOG(LogTemp, Error, TEXT("NavSystem is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return false;
	}
	//2. Patrol 거리가 너무 짧을 경우 다시 검색하기 위한 변수
	const int32 MaxAttemps = 5;
	const float MinDistance = Range * 0.3f;

	//3. CurrentLocation과 RandomPoint의 거리가 너무 짧을 경우 다시 계산
	for (int32 i = 0; i < MaxAttemps; ++i)
	{
		FNavLocation ResultLocation;
		bool bFindRandomPoint = NavSystem->GetRandomReachablePointInRadius(Origin, Range, ResultLocation);
		if (!bFindRandomPoint)
		{
			UE_LOG(LogTemp, Error, TEXT("RandomPoint is invalid : %d, %hs"), __LINE__, __FUNCTION__);
			continue;
		}
		float Distance = FVector::Dist(CurrentLocation, ResultLocation.Location);
		
		if (Distance >= MinDistance)
		{
			OutPoint = ResultLocation.Location;
			return true;
		}
	}

	return false;
}
