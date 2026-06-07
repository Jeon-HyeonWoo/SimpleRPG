// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_InitializeBlackboard.generated.h"

/**
 * AIController의 OnPossess에서 설정해도 되는 걸 왜 따로 Task로 뽑는가?
 * RunTime에서 설정되어야 할 BB의 Value들을 Cpp에서 하드코딩하고 싶지 않음. const FName도 생각했지만,
 * Editor내에서 이름 변경시 const FName의 이름또한 맞춰야 함.
 * 이러한 불편함들을 감수하고 BB Value의 Initalize Task를 만들어 처리하는 비용이 크지 않다고 판단.
 */
UCLASS()
class SIMPLERPG_API UBTTaskNode_InitializeBlackboard : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTaskNode_InitializeBlackboard();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//Selector를 쓰는 Node는 Override.
	//BlackBoard의 Key값이 어느 Type인지 모르기 때문에 사용하는 함수.
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
public:

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	FBlackboardKeySelector SpawnLocationKey;
	

};
