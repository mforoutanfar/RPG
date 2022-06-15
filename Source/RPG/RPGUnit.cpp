// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGUnit.h"

#include "RPGBillboardVisuals.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework\CharacterMovementComponent.h"

#include "RPG_MinimapComponent.h"

ARPGUnit::ARPGUnit()
{
	CreatureType = CreatureType::ENEMY;

	Visuals = CreateDefaultSubobject<URPGBillboardVisuals>(FName("Visuals"));
	Visuals->AttackCallback.BindUObject(this, &ARPGUnit::Attack);
	Visuals->SetupAttachment(RootComponent);

	MeleeSphere->SetCollisionProfileName(FName("EnemyDamageSource"));
	RangeSphere->SetCollisionProfileName(FName("EnemyDamageSource"));
	HitBox->SetCollisionProfileName(FName("EnemyHitBox"));

	MinimapComponent = CreateDefaultSubobject<URPG_MinimapComponent>(FName("MinimapComponent"));
}

void ARPGUnit::BeginPlay()
{
	Super::BeginPlay();
	Visuals->Init(CreatureName.ToString());
	SetIsWalking(false);

	switch (CreatureType)
	{
	case ARPGCreature::CreatureType::PLAYER:
		break;
	case ARPGCreature::CreatureType::ENEMY:
	{
		MinimapComponent->RegisterOnMiniMap(MiniMap::ENEMY);
		break;
	}
	case ARPGCreature::CreatureType::NPC:
	{
		MinimapComponent->RegisterOnMiniMap(MiniMap::NPC);
		break;
	}
	default:
		break;
	}
}

InteractableCat ARPGUnit::GetInteractableType_Implementation()
{
	if (Dead)
	{
		return InteractableCat::CORPSE;
	}

	return InteractableCat::NONE;
}

void ARPGUnit::OnInteracted_Implementation(bool Successful)
{
	if (Successful && Dead)
	{
		MinimapComponent->UnregisterFromMiniMap();
		Destroy();
	}
}

bool ARPGUnit::IsInteractable_Implementation()
{
	return Dead;
}

void ARPGUnit::SetIsWalking(bool IsWalking)
{
	Walking = IsWalking;
	Visuals->OnOwnerWalkingStateChanged(this, IsWalking);
}

void ARPGUnit::Die()
{
	Super::Die();
	if (Controller)
	{
		Controller->UnPossess();
	}

	//Removes threat to PlayerUnits (Affects safety)
	MeleeSphere->DestroyComponent();
	RangeSphere->DestroyComponent();

	//TODO: Replace Enemy ping with Loot ping.
	MinimapComponent->UnregisterFromMiniMap();
	MinimapComponent->RegisterOnMiniMap(MiniMap::LOOT);
}

void ARPGUnit::BeginAttack()
{
	Visuals->BeginAttack();

	//TODO: Hack! Entering recovery before actually attacking because otherwise behavior tree will misbehave.
	EnterRecovery(1.25f);
}
