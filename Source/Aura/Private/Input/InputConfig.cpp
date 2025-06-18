// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/InputConfig.h"

const UInputAction* UInputConfig::FindAbilityInputAction(const FGameplayTag AbilityTag) const
{
	for (const auto& [InputAction, InputTag] : InputActions)
	{
		if (InputAction && InputTag.MatchesTag(AbilityTag))
		{
			return InputAction;
		}
	}

	return nullptr;
}
