// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ProjectileSpellAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemReplicationProxyInterface.h"
#include "Actor/Projectile.h"
#include "Interaction/CombatInterface.h"

void UProjectileSpellAbility::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
		return;

	check(ProjectileClass);
	
	FVector SpawnPosition;
	if (const ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
		SpawnPosition = CombatInterface->GetCombatSocketLocation();//TODO exposing this would give more flexibility if reused by different abilities

	FRotator Rotation = (ProjectileTargetLocation - SpawnPosition).Rotation();
	
	FTransform SpawnTransform(SpawnPosition);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	Projectile->DamageEffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
	Projectile->FinishSpawning(SpawnTransform);
}
