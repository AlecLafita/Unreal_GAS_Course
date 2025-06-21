// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputConfig.h"
#include "BaseEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBaseEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()	
public:

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UBaseEnhancedInputComponent::BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	
	for (const auto& [InputAction, InputTag] : InputConfig->InputActions)
	{
		if (InputAction && InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputTag);
			}

			if (HeldFunc)
			{
				BindAction(InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputTag);
			}
		}
	}
}
