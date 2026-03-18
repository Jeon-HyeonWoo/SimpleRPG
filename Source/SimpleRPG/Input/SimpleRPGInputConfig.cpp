// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGInputConfig.h"
#include "InputAction.h"

const UInputAction* USimpleRPGInputConfig::FindNativeInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
    for (const FSimpleRPGInputAction& Action : NativeInputActions)
    {
        if (Action.InputAction && (Action.InputTag == InputTag))
        {
            return Action.InputAction;
        }
    }
    
    if (bLogNotFound)
    {
        UE_LOG(LogTemp, Error, TEXT("Can not find nativeinputaction for Inputtag [%s] on inputconfig[%s]"), *InputTag.ToString(), *GetNameSafe(this));
    }
    

    return nullptr;
}

const UInputAction* USimpleRPGInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
    for (const FSimpleRPGInputAction& Action : AbilityInputActions)
    {
        if (Action.InputAction && (Action.InputTag == InputTag))
        {
            return Action.InputAction;
        }
    }

    if (bLogNotFound)
    {
        UE_LOG(LogTemp, Error, TEXT("Can not find abilityinputaction for Inputtag [%s] on inputconfig[%s]"), *InputTag.ToString(), *GetNameSafe(this));
    }

    return nullptr;
}

ESimpleRPGAbilityInputID USimpleRPGInputConfig::FindAbilityInputIDByTag(const FGameplayTag& InputTag) const
{
    for (const FSimpleRPGInputAction& Action : AbilityInputActions)
    {
        if (Action.InputAction && (Action.InputTag == InputTag))
        {
            return Action.AbilityInputID;
        }
    }

    return ESimpleRPGAbilityInputID::None;
}
