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
	AEnemyCharacter();

	/* IHighlightable */
	virtual void Highlight() override;
	virtual void UnHighlight() override;
	/* ~IHighlightable */

	/* ICombatInterface */
	virtual int32 GetCombatLevel() const override {return Level;}
	/* ~ICombatInterface */

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,	Category = "Catergory Class Defaults")
	int32 Level = 1; //Don't replicate until we  it on client side
};
