// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGGameplayAbility.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "SimpleRPG/Character/SimpleRPGCharacterBase.h"
#include "SimpleRPG/AbilitySystem/Data/DamageDataTableRow.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SimpleRPG/AbilitySystem/SimpleRPGAttributeSet.h"

void USimpleRPGGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
    //1. CoolDown GE Class 가져오기
    UGameplayEffect* CoolDownGE = GetCooldownGameplayEffect();
    if (!CoolDownGE)
    {
        return;
    }

    //2. GESpec생성
    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(
        CoolDownGE->GetClass(),
        GetAbilityLevel()
    );
    if (!SpecHandle.IsValid())
    {
        return;
    }

    FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
    if (!Spec)
    {
        return;
    }
   
    const FDamageDataTableRow* Row = CoolDownDataRow.GetRow<FDamageDataTableRow>(TEXT("ApplyCoolDown"));
    if (!Row)
    {
        return;
    }
    
    float Duration = Row->CoolDown;

    //3. SetByCaller로 Duration 주입
    Spec->SetSetByCallerMagnitude(SimpleRPGGameplayTags::SetByCaller_CoolDown_Duration, Duration);

    Spec->DynamicGrantedTags.AppendTags(CoolDownTags);

    //4. 적용
    ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}

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

void USimpleRPGGameplayAbility::ApplyDamageToTarget(AActor* Target, TSubclassOf<UGameplayEffect> DamageEffect, float SkillRatio, FGameplayTag InflictedHitReaction)
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

    if (InflictedHitReaction.IsValid())
    {
        SpecHandle.Data->AddDynamicAssetTag(InflictedHitReaction);
    }

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
