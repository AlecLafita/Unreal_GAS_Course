// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/BaseEnhancedInputComponent.h"
#include "Interaction/Highlightable.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	MouseFollowSpline = CreateDefaultSubobject<USplineComponent>("MouseFollowSpline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(PlayerContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UBaseEnhancedInputComponent* EnhancedInputComponent = CastChecked<UBaseEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered,this, &AAuraPlayerController::Move);
	EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
		const FRotator Rotation =  GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
		return;

	PreviousHightlightable = CurrentHightlightable;
	CurrentHightlightable = CursorHit.GetActor();
	//CurrentHightlightable = Cast<IHighlightable>(CursorHit.GetActor()); //this would be needed if we store a raw pointer instead of a Script interface, seems a good way to avoid the Cast

	if (PreviousHightlightable == CurrentHightlightable)
		return;
	if (PreviousHightlightable == nullptr && CurrentHightlightable != nullptr)
		CurrentHightlightable->Highlight();
	else if (PreviousHightlightable != nullptr && CurrentHightlightable == nullptr)
		PreviousHightlightable->UnHighlight();
	else if (PreviousHightlightable != nullptr && CurrentHightlightable != nullptr)
	{
		PreviousHightlightable->UnHighlight();
		CurrentHightlightable->Highlight();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (UAuraAbilitySystemComponent* ASC = GetASC())
		ASC->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) || IsTargeting())
	{
		if (UAuraAbilitySystemComponent* ASC = GetASC())
			ASC->AbilityInputTagHeld(InputTag);
	}
	else
	{
		MouseFollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult CursorHit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, CursorHit))
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

 bool AAuraPlayerController::IsTargeting() const
{
	return CurrentHightlightable != nullptr;
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AuraAbilitySystemComponent;
}
