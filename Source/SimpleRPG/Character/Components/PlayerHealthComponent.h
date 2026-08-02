// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "PlayerHealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIMPLERPG_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerHealthComponent();


	void BindToAttributeSet();

public:

	UFUNCTION()
	void HandleDamaged(AActor* Instigator, FGameplayTagContainer AssetTags);

	UFUNCTION()
	void HandleDeath(AActor* Instigator);

};