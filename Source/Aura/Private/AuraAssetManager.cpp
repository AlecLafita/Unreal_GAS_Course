// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	return *Cast<UAuraAssetManager>(GEngine->AssetManager);
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	//Apparently this is no longer needed from 5.3 on https://github.com/tranek/GASDocumentation?tab=readme-ov-file#491-initglobaldata
	//UAbilitySystemGlobals::Get().InitGlobalData(); //Needed for use TargetData for replication! Otherwise cache won't be initialized and everything will be fucked up when trying to access it
}