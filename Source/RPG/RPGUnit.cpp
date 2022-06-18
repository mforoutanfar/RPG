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

void ARPGUnit::RPGSetActive(bool Active)
{
	SetActorHiddenInGame(!Active);
	SetActorTickEnabled(Active);
	GetCharacterMovement()->SetComponentTickEnabled(Active);
	SetActorEnableCollision(Active);
	GetCapsuleComponent()->SetEnableGravity(Active);

	if (Active)
	{
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

		SpawnDefaultController();

		GetCharacterMovement()->GravityScale = 1.0f;

		Dead = false;

		MeleeSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		RangeSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		Visuals->SetAnimState(AnimState::AST_IDLE);

		HP = MaxHP;
		Mana = MaxMana;

		AddStartingInventoryItems();
	}	
	else
	{
		MinimapComponent->UnregisterComponent();
		if (Controller)
		{
			Controller->UnPossess();
		}

		GetCharacterMovement()->GravityScale = 0.0f;
	}
}

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

	RPGSetActive(false);
}

void ARPGUnit::BeginPlay()
{
	Super::BeginPlay();
	Visuals->Init(CreatureName.ToString());
	SetIsWalking(false);
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
		RPGEventManager->CreatureExpired.Broadcast(this);
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
	MeleeSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RangeSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
