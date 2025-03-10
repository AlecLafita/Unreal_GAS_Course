// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/Highlightable.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemyCharacter : public ACharacterBase, public IHighlightable
{
	GENERATED_BODY()

public:
	virtual void Highlight() override;
	virtual void UnHighlight() override;
};
