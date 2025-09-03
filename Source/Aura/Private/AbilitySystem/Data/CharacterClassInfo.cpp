// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterClassInfo.h"

const FCharacterClassDefaultsInfo& UCharacterClassInfo::GetCharacterClassDefaultsInfo(ECharacterClass CharacterClass)
{
	return CharacterClassDefaults.FindChecked(CharacterClass);
}
