// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateTarget.generated.h"

/**
 * 
 */

class UAIPerceptionComponent;

UCLASS()
class SIMPLERPG_API UBTService_UpdateTarget : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_UpdateTarget();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

private:
	
	AActor* FindClosestPerceivedActor(UAIPerceptionComponent* PerceptionComp, const FVector& Origin, float& OutDistance);

public:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetDistanceKey;
};
