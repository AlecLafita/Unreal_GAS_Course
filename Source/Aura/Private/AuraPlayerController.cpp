// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/BaseEnhancedInputComponent.h"
#include "Interaction/Highlightable.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	CursorFollowSpline = CreateDefaultSubobject<USplineComponent>("CursorFollowSpline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
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
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
		return;

	PreviousHightlightable = CurrentHightlightable;
	CurrentHightlightable = CursorHit.GetActor();
	//CurrentHightlightable = Cast<IHighlightable>(CursorHit.GetActor()); //this would be needed if we store a raw pointer instead of a Script interface, seems a good way to avoid the Cast

	if (PreviousHightlightable == CurrentHightlightable)
		return;
	
	if (CurrentHightlightable != nullptr)
		CurrentHightlightable->Highlight();
	if (PreviousHightlightable != nullptr)
		PreviousHightlightable->UnHighlight();
}

void AAuraPlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bPressedTarget = CurrentHightlightable != nullptr;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) || bPressedTarget)
	{
		if (UAuraAbilitySystemComponent* ASC = GetASC())
			ASC->AbilityInputTagReleased(InputTag);
	}
	else
	{
		const APawn* ControlledPawn = GetPawn<APawn>();
		if (CursorFollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CursorFollowDestination))
			{
				CursorFollowSpline->ClearSplinePoints();
				for (const FVector& PathPoint : NavPath->PathPoints)
				{
					CursorFollowSpline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
				}
				if (!NavPath->PathPoints.IsEmpty())
					CursorFollowDestination = NavPath->PathPoints.Last();
				bAutoRunning = true;
			}
		}
		CursorFollowTime = 0.f;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) || bPressedTarget)
	{
		if (UAuraAbilitySystemComponent* ASC = GetASC())
			ASC->AbilityInputTagHeld(InputTag);
	}
	else
	{
		CursorFollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit)
			CursorFollowDestination = CursorHit.ImpactPoint;

		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			const FVector WorldDirection = (CursorFollowDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning)
		return;
	
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = CursorFollowSpline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector Direction = CursorFollowSpline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (CursorFollowDestination - LocationOnSpline).Length();
		if (DistanceToDestination < AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;	
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AuraAbilitySystemComponent;
}
