// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGGameplayAbility.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "SimpleRPG/Character/SimpleRPGCharacterBase.h"
#include "SimpleRPG/AbilitySystem/Data/DamageDataTableRow.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

ASimpleRPGCharacterBase* USimpleRPGGameplayAbility::GetOwnerCharacter() const
{
    ASimpleRPGCharacterBase* OwnerCharacter = Cast<ASimpleRPGCharacterBase>(GetActorInfo().AvatarActor.Get());
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("OwnerCharacter is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
    }

    return OwnerCharacter;
}

UAbilitySystemComponent* USimpleRPGGameplayAbility::GetSimpleRPGASC() const
{
    return GetAbilitySystemComponentFromActorInfo();
}

bool USimpleRPGGameplayAbility::GetSkillRatio(UDataTable* DataTable, FName RowName, float& OutRatio) const
{
    //0. init Outratio 
    OutRatio = 0.0f;

    //1. DT null check
    if (!ensureMsgf(IsValid(DataTable), TEXT("Data Table is invalid")))
    {
        return false;
    }

    //2. Row null check
    FDamageDataTableRow* Row = DataTable->FindRow<FDamageDataTableRow>(RowName, TEXT(""));
    if (!ensureMsgf(Row, TEXT("Row is invalid")))
    {
        return false;
    }
    
    //3. GetRatio
    OutRatio = Row->SkillRatio;

    return true;
}

void USimpleRPGGameplayAbility::ApplyDamageToTarget(AActor* Target, TSubclassOf<UGameplayEffect> DamageEffect, float SkillRatio)
{
    //1. Target Actor null check  
    if (!Target)
    {
        UE_LOG(LogTemp, Warning, TEXT("Target Actor is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
        return;
    }

    //2. DamageEffect null check
    if (!ensureMsgf(IsValid(DamageEffect), TEXT("DamageEffect invalid")))
    {
        return;
    }
    
    //3. Get TargetASC
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
    if (!TargetASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetASC is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
        return;
    }
    
    //4. MakeOutGoingGameplayEffectSpec (DamageEffect, 1.0f)
    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect, 1.0f);
    if (!ensureMsgf(SpecHandle.IsValid(), TEXT("SpecHandle is invalid")))
    {
        return;
    }

    SpecHandle.Data->SetSetByCallerMagnitude(
        SimpleRPGGameplayTags::SetByCaller_Skill_Ratio,
        SkillRatio
    );

    //5. Apply Damage to TargetASC
    GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *SpecHandle.Data.Get(),
        TargetASC
    );
}
