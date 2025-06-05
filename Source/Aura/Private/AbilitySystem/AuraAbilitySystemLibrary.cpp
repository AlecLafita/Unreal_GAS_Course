// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include <Kismet/GameplayStatics.h>

#include "Player/AuraPlayerState.h"
#include "UI/HUD/BaseHUD.h"
#include "UI/WisgetController/BaseWidgetController.h"


ABaseHUD* CreateWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ABaseHUD* HUD = Cast<ABaseHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState< AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutParams = FWidgetControllerParams{ PC, PS, ASC, AS };
			return HUD;
		}
	}

	return nullptr;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetParams;
	if (ABaseHUD * HUD = CreateWidgetControllerParams(WorldContextObject, WidgetParams))
		return HUD->GetOverlayWidgetController(WidgetParams);

	return nullptr;
}


UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetParams;
	if (ABaseHUD * HUD = CreateWidgetControllerParams(WorldContextObject, WidgetParams))
		return HUD->GetAttributeMenuWidgetController(WidgetParams);

	return nullptr;
}
