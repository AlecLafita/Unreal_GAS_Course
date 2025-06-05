// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include <Kismet/GameplayStatics.h>

#include "Player/AuraPlayerState.h"
#include "UI/HUD/BaseHUD.h"
#include "UI/WisgetController/BaseWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ABaseHUD* BaseHUD = Cast<ABaseHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState< AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			return BaseHUD->GetOverlayWidgetController({PC, PS, ASC, AS});
		}
	}

	return nullptr;
}
