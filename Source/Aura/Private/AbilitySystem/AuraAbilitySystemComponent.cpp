// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UBaseGameplayAbility* Ability = Cast<UBaseGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(Ability->StartupInputTag);
		}
		GiveAbility(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
		return;

	TArray<FGameplayAbilitySpec>& ActivableAbilities = GetActivatableAbilities();
	for (FGameplayAbilitySpec& ActivableAbility : ActivableAbilities)
	{
		AbilitySpecInputPressed(ActivableAbility); //Can call a InputPressed callback on ability
		if (ActivableAbility.GetDynamicSpecSourceTags().HasTagExact(InputTag) && !ActivableAbility.IsActive())
		{
			TryActivateAbility(ActivableAbility.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
		return;

	TArray<FGameplayAbilitySpec>& ActivableAbilities = GetActivatableAbilities();
	for (FGameplayAbilitySpec& ActivableAbility : ActivableAbilities)
	{
		if (ActivableAbility.GetDynamicSpecSourceTags().HasTagExact(InputTag))
			AbilitySpecInputReleased(ActivableAbility);
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer Tags;
	EffectSpec.GetAllAssetTags(Tags);
	EffectAssetTagsDelegate.Broadcast(Tags);
}
