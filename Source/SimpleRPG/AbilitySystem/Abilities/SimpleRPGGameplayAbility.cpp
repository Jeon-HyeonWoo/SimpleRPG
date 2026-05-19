// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGGameplayAbility.h"
#include "SimpleRPG/Character/SimpleRPGCharacterBase.h"

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
