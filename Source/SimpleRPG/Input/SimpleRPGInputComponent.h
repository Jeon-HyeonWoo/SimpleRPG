// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "SimpleRPGInputConfig.h"
#include "SimpleRPGInputComponent.generated.h"

/**
 * 
 */

UCLASS()
class SIMPLERPG_API USimpleRPGInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	template<class UserClass, typename FuncType>
	void BindNativeAction(
		const USimpleRPGInputConfig* InputConfig,
		const FGameplayTag& InputTag,
		ETriggerEvent TriggerEvent,
		UserClass* Object,
		FuncType Func
		);

	template<class UserClass>
	void BindAbilityActions(
		const USimpleRPGInputConfig* InputConfig,
		UserClass* Object,
		void(UserClass::* PressedFunc)(const FInputActionValue&, FGameplayTag),
		void(UserClass::* ReleasedFunc)(const FInputActionValue&, FGameplayTag)
	);
};

template<class UserClass, typename FuncType>
inline void USimpleRPGInputComponent::BindNativeAction(const USimpleRPGInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);

	if (const UInputAction* IA = InputConfig->FindNativeInputActionByTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func, InputTag);
	}
}



template<class UserClass>
inline void USimpleRPGInputComponent::BindAbilityActions(const USimpleRPGInputConfig* InputConfig, UserClass* Object, void(UserClass::* PressedFunc)(const FInputActionValue&, FGameplayTag), void(UserClass::* ReleasedFunc)(const FInputActionValue&, FGameplayTag))
{
	check(InputConfig);

	for (const FSimpleRPGInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (PressedFunc)
		{
			BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag);
		}
		if (ReleasedFunc)
		{
			BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
		}
	}
}
