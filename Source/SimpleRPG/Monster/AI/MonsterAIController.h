// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	virtual void OnPossess(APawn* InPawn) override;


public:

};
