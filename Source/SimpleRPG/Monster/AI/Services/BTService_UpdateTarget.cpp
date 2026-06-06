// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateTarget.h"
#include "SimpleRPG/Monster/AI/MonsterAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SimpleRPG/Monster/Data/MonsterData.h"

UBTService_UpdateTarget::UBTService_UpdateTarget()
{
	NodeName = "Update Target";
}

void UBTService_UpdateTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Get AIController
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//Get PerceptionComponent
	UAIPerceptionComponent* PerceptionComp = AIController->GetPerceptionComponent();
	if (!PerceptionComp)
	{
		UE_LOG(LogTemp, Error, TEXT("PerceptionComponent is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//Get Blackboard
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BlackBoardComponent is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//Get OwnerActor
	ASimpleRPGMonsterBase* MonsterBase = Cast<ASimpleRPGMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MonsterBase)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterBase is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//Get MonsterBaseData
	UMonsterData* MonsterData = MonsterBase->GetMonsterData();
	if (!MonsterData)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterData is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//Get PercivedActors
	TArray<AActor*> PerceivedActors;

	AActor* ClosestActor = nullptr;
	float ClosestDist = 0.0f;
	FVector OwnerLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

	ClosestActor = FindClosestPerceivedActor(PerceptionComp, OwnerLocation, ClosestDist);

	if (ClosestActor != nullptr)
	{
		BBComp->SetValueAsObject(TargetActorKey.SelectedKeyName, ClosestActor);
		BBComp->SetValueAsFloat(TargetDistanceKey.SelectedKeyName, ClosestDist);
		AIController->SetFocus(ClosestActor);
	}
	else
	{
		BBComp->ClearValue(TargetActorKey.SelectedKeyName);
		BBComp->ClearValue(TargetDistanceKey.SelectedKeyName);
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
	
}

AActor* UBTService_UpdateTarget::FindClosestPerceivedActor(UAIPerceptionComponent* PerceptionComp, const FVector& Origin, float& OutDistance)
{
	OutDistance = 0.0f;

	//인식된 액터들을 담는 배열
	TArray<AActor*> PerceivedActors;
	//인식 방법 : Sight(시야)에 담긴 액터들을 Array에 담는 함수
	PerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

	//인식된 액터들이 없다면 nullptr 반환
	if (PerceivedActors.IsEmpty())
	{
		return nullptr;
	}

	float ClosestDist = MAX_FLT;
	AActor* ClosestActor = nullptr;

	//인식된 액터들을 순회하면서 가장 가까운 액터, 거리를 갱신
	for (const auto& Actor : PerceivedActors)
	{
		if (!Actor) continue;

		//Origin(OwnerActor)와 감지된 Actor의 거리 
		float Dist = FVector::Dist(Origin, Actor->GetActorLocation());

		//가장 가까운 거리 값, 가장 가까운 액터 갱신
		if (Dist < ClosestDist)
		{
			ClosestDist = Dist;
			ClosestActor = Actor;
		}
		
	}

	//순회 이후 가장 가까운 액터가 있다면 그 액터의 거리를 대입
	if (ClosestActor)
	{
		OutDistance = ClosestDist;
	}

	//가장 가까운 액터 반환
	return ClosestActor;
}


