// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "SimpleRPG/Monster/Data/MonsterData.h"
#include "BehaviorTree/BehaviorTree.h"

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//InPawn valid check
	if (!InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("InPawn is unvalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}


	//Cast Monster Character, valid check
	ASimpleRPGMonsterBase* MonsterPawn = Cast<ASimpleRPGMonsterBase>(InPawn);
	if (!MonsterPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterPawn is unvalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//MonsterData valid check
	UMonsterData* MonsterData = MonsterPawn->GetMonsterData();
	if (!MonsterData)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterData is unvalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//BehaviorTree valid check
	UBehaviorTree* BehaviorTree = MonsterData->BehaviorTree;
	if (!BehaviorTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree is unvalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//Use Bloackboard俊辑 积己等 BlackBoardComponent 积己
	UBlackboardComponent* BBComponent = nullptr;

	UseBlackboard(BehaviorTree->BlackboardAsset, BBComponent);

	RunBehaviorTree(BehaviorTree);
}
