// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimpleRPGPlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class USimpleRPGInputConfig;

UCLASS()
class SIMPLERPG_API ASimpleRPGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ASimpleRPGPlayerController();

	virtual void BeginPlay() override;

	/* InputComponent가 준비가 되었을 때 호출 */
	virtual void SetupInputComponent() override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	/* IMC 등록시 Priority가 필요 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Input")
	int32 InputMappingContextPriority = 0;

};
