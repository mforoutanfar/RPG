// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGUnit.h"

#include "RPGBillboardVisuals.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework\CharacterMovementComponent.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ARPGUnit::ARPGUnit()
{
	CreatureType = CreatureType::ENEMY;

	Visuals = CreateDefaultSubobject<URPGBillboardVisuals>(FName("Visuals"));
	Visuals->SetupAttachment(RootComponent);

	MeleeSphere->SetCollisionProfileName(FName("EnemyDamageSource"));
	RangeSphere->SetCollisionProfileName(FName("EnemyDamageSource"));
	HitBox->SetCollisionProfileName(FName("EnemyHitBox"));

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("PerceptionComponent"));	

	auto SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("SightConfig"));

	SightConfig->SightRadius = 7000.0f;
	SightConfig->LoseSightRadius = 8000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 142.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->SetMaxAge(45.0f);

	PerceptionComponent->ConfigureSense(*SightConfig);
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
		RegisterOnMiniMap(this, MiniMap::ENEMY);
		break;
	}
	case ARPGCreature::CreatureType::NPC:
	{
		RegisterOnMiniMap(this, MiniMap::NPC);
		break;
	}
	default:
		break;
	}
}

InteractableCat ARPGUnit::GetInteractableType()
{
	if (Dead)
	{
		return InteractableCat::CORPSE;
	}

	return InteractableCat::NONE;
}

void ARPGUnit::OnInteracted(bool Successful)
{
	if (Dead)
	{
		Destroy();
	}
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

	//TODO: Replace Enemy ping with Loot ping.
	UnregisterFromMiniMap(this);
}

