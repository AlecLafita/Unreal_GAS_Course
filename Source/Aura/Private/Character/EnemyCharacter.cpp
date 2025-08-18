// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Chaos/Deformable/MuscleActivationConstraints.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/BaseUserWidget.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);//Enemies don't need to have effects replicated as no player is controlling them 

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBarComponent");
	HealthBarComponent->SetupAttachment(GetRootComponent());
}

void AEnemyCharacter::Highlight()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AEnemyCharacter::UnHighlight()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED); //TODO ideally this should be exposed. Highlight logic should be somewhere else more generic so we can apply different materials 
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (UBaseUserWidget* HealthBarWidgetObject = Cast<UBaseUserWidget>(HealthBarComponent->GetUserWidgetObject()))
	{
		HealthBarWidgetObject->SetWidgetController(this);
	}

	const FWidgetControllerParams WCParams(nullptr, nullptr, AbilitySystemComponent, AttributeSet);
	
	if (const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});

		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}
