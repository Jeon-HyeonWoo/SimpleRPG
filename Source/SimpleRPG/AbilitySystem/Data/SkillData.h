// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SkillTargetingType.h"
#include "SkillData.generated.h"

class UTexture2D;
class UAnimMontage;
class USimpleRPGGameplayAbility;
struct FSkillDataTableRow;

/**
 * 
 */
UCLASS(BlueprintType, Const, meta = (DisplayName = "SimpleRPG Skill Data"))
class SIMPLERPG_API USkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

#pragma region Presentation

	/* 사용자에게 노출되는 네임 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Presentation")
	FText SkillDisplayName;

	/* 커서 오버 시 노출되는 스킬 설명 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Presentation", meta = (MultiLine = "true"))
	FText SkillDescription;

	/* 스킬 아이콘 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Presentation")
	TSoftObjectPtr<UTexture2D> SkillICon;

#pragma endregion


#pragma region Balancing

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Balancing")
	FDataTableRowHandle BalancingDataRow;

#pragma endregion


#pragma region Logic

	/* Blueprint GA Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Logic")
	TSubclassOf<USimpleRPGGameplayAbility> LogicAbilityClass;

	/* 사용 방법 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Logic")
	ESkillTargettingType TargetingType = ESkillTargettingType::Immediate;

#pragma endregion

#pragma region Condition

	/* Active, Passive 등 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Condition", meta = (Categories = "Skill.Category"))
	FGameplayTag SkillCategory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Condition", meta = (Categories = "Weapon.Type"))
	FGameplayTagContainer RequireWeaponTags;

#pragma endregion


#pragma region Helper

public:

	const FSkillDataTableRow* GetSkillDataTableRow() const;

	UFUNCTION(BlueprintPure, Category = "SimpleRPG|Skill")
	bool CanUseWithWeaponTags(const FGameplayTagContainer& InWeaponTags) const;

#pragma endregion

};
