// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpleRPGGameMode.generated.h"

/**
 * 
 */

class ASimpleRPGPlayerState;
class UPawnData;
/*
* 게임 규칙을 결정하는 GameMode
* 서버 권위적으로 PawnData를 PlayerState에 전달
*/
UCLASS()
class SIMPLERPG_API ASimpleRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ASimpleRPGGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	/* 새 플레이어 참가 시 PawnData를 PlayerState에 전달 */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

public:

	/* 이 게임모드에서 사용할 기본 PawnData */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Pawn")
	TObjectPtr<UPawnData> DefaultPawnData;
};
