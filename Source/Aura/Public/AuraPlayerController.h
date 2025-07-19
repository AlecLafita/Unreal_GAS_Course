// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UInputConfig;
struct FInputActionValue;
class IHighlightable;
class UInputAction;
class UInputMappingContext;
class UAuraAbilitySystemComponent;
class USplineComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	void OnShiftPressed() {bShiftKeyDown = true;}
	void OnShiftReleased() {bShiftKeyDown = false;}
	
	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	FHitResult CursorHit;
	TScriptInterface<IHighlightable> PreviousHightlightable = nullptr;
	TScriptInterface<IHighlightable> CurrentHightlightable = nullptr;

	void AbilityInputTagPressed(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);

	void AutoRun();
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	UAuraAbilitySystemComponent* GetASC();
	
	FVector CursorFollowDestination = FVector::ZeroVector;
	float CursorFollowTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	float ShortPressThreshold = 0.5f;

	bool bPressedTarget = false;
	bool bAutoRunning = false;
	bool bShiftKeyDown = false;

	const bool CanMoveFromInput(const FGameplayTag InputTag) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere, Category="Input")
	TObjectPtr<USplineComponent> CursorFollowSpline;
};
