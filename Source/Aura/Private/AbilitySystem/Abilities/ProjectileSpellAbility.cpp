// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ProjectileSpellAbility.h"

#include "Actor/Projectile.h"
#include "Interaction/CombatInterface.h"

void UProjectileSpellAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthority(&ActivationInfo))
		return;

	check(ProjectileClass);
	
	FVector SpawnPosition;
	if (const ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
		SpawnPosition = CombatInterface->GetCombatSocketLocation();//TODO exposing this would give more flexibility if reused by different abilities
	
	FTransform SpawnTransform(SpawnPosition); //TODO set also rotation
	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	//TODO give the projectile a GE for causing damage

	Projectile->FinishSpawning(SpawnTransform);
}
