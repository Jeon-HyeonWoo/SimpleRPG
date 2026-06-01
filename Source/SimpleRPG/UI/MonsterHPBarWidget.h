// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPBarWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SIMPLERPG_API UMonsterHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetHP(float Current, float Max);

public:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* MonsterHPBar;
};
