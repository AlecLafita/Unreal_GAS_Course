// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS(Abstract)
class AURA_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

	
protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable, Category="Combat")
	FName WeaponSocket;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
