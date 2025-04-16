// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

FEffectProperties::FEffectProperties(const FGameplayEffectModCallbackData& Data)
{
	//Source = causer of the effect, Target = target of the effect (owner of this AttributeSet)
	EffectContextHandle = Data.EffectSpec.GetContext();
	SourceInfo.ASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	check(SourceInfo.ASC);
	
	SourceInfo.AvatarActor = SourceInfo.ASC->GetAvatarActor();
	SourceInfo.Controller = SourceInfo.ASC->AbilityActorInfo->PlayerController.Get();
	if (SourceInfo.Controller == nullptr && SourceInfo.Controller != nullptr)
	{
		if (const APawn* Pawn = Cast<APawn>(SourceInfo.AvatarActor))
		{
			SourceInfo.Controller = Pawn->GetController();
		}
	}

	if (SourceInfo.Controller != nullptr)
	{
		SourceInfo.Character = Cast<ACharacter>(SourceInfo.Controller->GetPawn());
	}

	if (Data.Target.AbilityActorInfo.IsValid())
	{
		TargetInfo.AvatarActor = Data.Target.GetAvatarActor();
		TargetInfo.Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetInfo.Character = Cast<ACharacter>(TargetInfo.AvatarActor);
		TargetInfo.ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetInfo.AvatarActor);
	}
}

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);	
	InitMaxHealth(100.f);	
	InitMana(20.f);	
	InitMaxMana(50.f);	
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Needed to replicate any variable, we must register them
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

//This should just be used to clamp values and similar operations, never for game logic! 
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	else if (Attribute == GetManaAttribute())
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties(Data);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}
