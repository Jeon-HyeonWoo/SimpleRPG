// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "SimpleRPG/Monster/Data/MonsterData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"

AMonsterAIController::AMonsterAIController()
{
	//Create PerceptionComponent
	{
		PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
		if (!PerceptionComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("PerceptionComponent is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
			return;
		}
	}
	
	SetupSightSense();
	SetupDamageSense();

}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//InPawn valid check
	if (!InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("InPawn is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}


	//Cast Monster Character, valid check
	ASimpleRPGMonsterBase* MonsterPawn = Cast<ASimpleRPGMonsterBase>(InPawn);
	if (!MonsterPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterPawn is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//MonsterData valid check
	UMonsterData* MonsterData = MonsterPawn->GetMonsterData();
	if (!MonsterData)
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterData is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//BehaviorTree valid check
	UBehaviorTree* BehaviorTree = MonsterData->BehaviorTree;
	if (!BehaviorTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//Use Bloackboard에서 생성된 BlackBoardComponent 생성
	UBlackboardComponent* BBComponent = nullptr;

	UseBlackboard(BehaviorTree->BlackboardAsset, BBComponent);

	RunBehaviorTree(BehaviorTree);

}

void AMonsterAIController::SetupSightSense()
{
	//Create SenseConfig Sight and valid check
	{
		SenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfigSight"));
		if (!IsValid(SenseConfigSight))
		{
			UE_LOG(LogTemp, Error, TEXT("SenseConfigSight is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
			return;
		}
	}
	
	//Setup
	{
		SenseConfigSight->SightRadius = 1000.0f;								//감지범위
		SenseConfigSight->LoseSightRadius = 1500.0f;							//감지해제범위
		SenseConfigSight->PeripheralVisionAngleDegrees = 360.0f;				//감지 각도
		//TODO : IGenericTeamAgentInterface 설정 이후 중립, 아군 감지 해제
		SenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;			//적 감지
		SenseConfigSight->DetectionByAffiliation.bDetectNeutrals = true;		//중립 감지
		SenseConfigSight->DetectionByAffiliation.bDetectFriendlies = true;		//아군 감지
	}

	//Reigst SightSense with PerceptionComponent
	{
		PerceptionComponent->ConfigureSense(*SenseConfigSight);
		PerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	}
}

void AMonsterAIController::SetupDamageSense()
{
	SenseConfigDamage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("SenseConfig_Damage"));
	if (!IsValid(SenseConfigDamage))
	{
		UE_LOG(LogTemp, Error, TEXT("DamageConfigSense is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	{
		PerceptionComponent->ConfigureSense(*SenseConfigDamage);
	}
}
