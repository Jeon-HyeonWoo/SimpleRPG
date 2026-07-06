// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGGameplayAbility.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "SimpleRPG/Character/SimpleRPGCharacterBase.h"
#include "SimpleRPG/AbilitySystem/Data/DamageDataTableRow.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

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

bool USimpleRPGGameplayAbility::GetSkillRatio(const FDataTableRowHandle& DTRowHandle, float& OutRatio) const
{
    //1. init Outratio 
    OutRatio = 0.0f;

    //2. Row null check 
    FDamageDataTableRow* Row = DTRowHandle.GetRow<FDamageDataTableRow>(TEXT(""));
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

void USimpleRPGGameplayAbility::ApplyBlockMovement()
{
    //지정 한 한 GA는 조용히 무시
    if (!BlockMovementEffect) return;

    //이미 적용중이라면 무시
    if (BlockMovementEffectHandle.IsValid()) return;

    //GE 적용 및 Handle에 저장
    BlockMovementEffectHandle = ApplyGameplayEffectToOwner(
        CurrentSpecHandle,
        CurrentActorInfo,
        CurrentActivationInfo,
        BlockMovementEffect.GetDefaultObject(),
        1.0f
    );

    if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
    {
        Character->GetCharacterMovement()->bOrientRotationToMovement = false;
    }
}

void USimpleRPGGameplayAbility::RemoveBlockMovement()
{
    if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
    {
        Character->GetCharacterMovement()->bOrientRotationToMovement = true;
    }

    if (BlockMovementEffectHandle.IsValid())
    {
        if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
        {
            ASC->RemoveActiveGameplayEffect(BlockMovementEffectHandle);
            BlockMovementEffectHandle.Invalidate();
        }
    }
}
