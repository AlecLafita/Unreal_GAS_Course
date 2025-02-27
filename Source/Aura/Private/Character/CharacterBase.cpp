// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}