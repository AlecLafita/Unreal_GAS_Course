// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WisgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (const auto& [AttributeTag, GetAttributeFunction]: AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(AttributeTag, GetAttributeFunction());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (const auto& [AttributeTag, GetAttributeFunction]: AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAttributeFunction()).AddLambda(
			[this, AttributeTag, GetAttributeFunction] (const FOnAttributeChangeData& data)
			{
				BroadcastAttributeInfo(AttributeTag, GetAttributeFunction());
			});
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfo(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AS);
	AttributeInfoDelegate.Broadcast(Info);
}
