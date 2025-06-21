// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputConfig.generated.h"

USTRUCT(BlueprintType)
struct FBaseInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class AURA_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	const UInputAction* FindAbilityInputAction(const FGameplayTag AbilityTag) const;
	
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite)
	TArray<FBaseInputAction> InputActions; //TODO Probably better to have a map and some accessor functions
};
