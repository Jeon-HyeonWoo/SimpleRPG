// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "SimpleRPGPlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class USimpleRPGInputConfig;

UCLASS()
class SIMPLERPG_API ASimpleRPGPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:

	ASimpleRPGPlayerController();

	//PlayerController
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	//Team
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	/* IMC 등록시 Priority가 필요 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Input")
	int32 InputMappingContextPriority = 0;

};
