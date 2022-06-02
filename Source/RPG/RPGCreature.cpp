// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCreature.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "RPGFunctionLibrary.h"
#include "RPGRandomAudioComponent.h"

#include "RPG_GameStateBase.h"
#include "RPGPlayerUnit.h"
#include "RPGInventory.h"
#include "RPG_Equipment.h"
#include "RPG_Projectile.h"

#include "RPG_EventManager.h"
#include "RPG_ItemWidget.h"
#include "RPGInventoryItem.h"

// Sets default values
ARPGCreature::ARPGCreature()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->DestroyComponent();
	GetMesh()->SetActive(false);

	MeleeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeSphere"));
	MeleeSphere->SetupAttachment(RootComponent);

	RangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
	RangeSphere->SetupAttachment(RootComponent);

	HitBox = GetCapsuleComponent();

	AudioComponent = CreateDefaultSubobject<URPGRandomAudioComponent>(FName("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	Inventory = CreateDefaultSubobject<URPGInventory>(FName("Inventory"));
	Inventory->OwnerUnit = this;

	Equipment = CreateDefaultSubobject<URPG_Equipment>(FName("Equipment"));
	Equipment->OwnerUnit = this;
}

// Called when the game starts or when spawned
void ARPGCreature::BeginPlay()
{
	Super::BeginPlay();

	RPGEventManager->ItemWidgetPicked.AddDynamic(this, &ARPGCreature::OnItemWidgetPicked);
}

void ARPGCreature::OnItemWidgetPicked(URPG_ItemWidget* Item)
{
	if (auto ref = Item->ItemRef)
	{
		if (Inventory->Contains(ref))
		{
			Inventory->RemoveItem(ref);
		}
		else if (Equipment->Contains(ref))
		{
			Equipment->RemoveItem(ref);
		}
	}
}

// Called every frame
void ARPGCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FRPGAttackResults ARPGCreature::OnAttacked(FRPGAttackData AttackData)
{
	FRPGAttackResults Results;
	Results.Target = this;

	float DamageDealt = 0.0f;
	DamageDealt += AttackData.Damage;
	HP -= DamageDealt;

	Results.DamageDealt = DamageDealt;

	AudioComponent->PlayRandom("hit");

	float RecoveryDuration = 1.0f;

	if (HP <= 0)
	{
		Die();
		Results.TargetDied = true;
		RecoveryDuration = -1.0f;
	}

	EnterRecovery(RecoveryDuration);

	return Results;
}


void ARPGCreature::BeginAttack()
{
	auto Results = Attack();
	EnterRecovery(Results.RecoveryDuration);
}

FRPGAttackResults ARPGCreature::Attack()
{
	FRPGAttackData AttackData;
	FRPGAttackResults Results;
	float RecoveryDuration = 1.0f;

	bool ShouldBeVisible = true;
	if (CreatureType == CreatureType::ENEMY)
	{
		ShouldBeVisible = false;
	}
	bool ExcludeOwnType = true;

	if (auto NearestMelee = GetNearestAttackTarget(MeleeSphere, ExcludeOwnType, ShouldBeVisible))
	{
		AudioComponent->PlayRandom("whoosh");

		AttackData.Attacker = this;
		AttackData.Target = NearestMelee;
		AttackData.Damage = MeleeDamage;

		auto Attackable = Cast<IRPGAttackable>(NearestMelee);
		Results = Attackable->OnAttacked(AttackData);
	}
	else
	{
		if (auto NearestRanged = GetNearestAttackTarget(RangeSphere, ExcludeOwnType, ShouldBeVisible))
		{
			AudioComponent->PlayRandom("arrow_shot");

			auto ModifActionLocation = GetActorRotation().RotateVector(ActionLocation);
			auto PointOfAction = GetActorLocation() + ModifActionLocation;
			auto Direction = (NearestRanged->GetActorLocation() - PointOfAction).Rotation();
			auto Projectile = GetWorld()->SpawnActor<ARPG_Projectile>(ProjectileClass, PointOfAction, Direction);
			Results.Ranged = true;
		}
		else
		{
			AudioComponent->PlayRandom("whoosh");
		}
	}

	Results.RecoveryDuration = RecoveryDuration;

	RPGEventManager->AttackOccured.Broadcast(this, AttackData.Target, Results);

	return Results;
}

void ARPGCreature::Die()
{
	Dead = true;
	AttackableEnabled = false;
}

AActor* ARPGCreature::GetNearestAttackTarget(UShapeComponent* Collider, bool ExcludeOwnType, bool ShouldBeVisible)
{
	AActor* ClosestAttackableActor = nullptr;
	float MinDistance = FLT_MAX;
	TSet<AActor*> OverlappingActors = {};
	Collider->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (ShouldBeVisible)
		{
			FVector2D pos;
			if (!UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(this, 0), Actor->GetActorLocation(), pos))
			{
				continue;
			}
		}

		if (auto Attackable = Cast<IRPGAttackable>(Actor))
		{
			if (!Attackable->AttackableEnabled)
			{
				continue;
			}
		}

		if (auto Creature = Cast<ARPGCreature>(Actor))
		{
			if (ExcludeOwnType && Creature->CreatureType == CreatureType)
			{
				continue;
			}
		}

		auto Distance = (Actor->GetActorLocation() - GetActorLocation()).SizeSquared();
		if (Distance < MinDistance)
		{
			ClosestAttackableActor = Actor;
			MinDistance = Distance;
		}
	}

	return ClosestAttackableActor;
}

void ARPGCreature::EnterRecovery(float Duration)
{
	GetWorldTimerManager().ClearTimer(RecoveryTimerHandle);

	InRecovery = true;

	if (Duration >= 0)
	{
		GetWorldTimerManager().SetTimer(RecoveryTimerHandle, this, &ARPGCreature::ExitRecovery, Duration, false);
	}

	RPGEventManager->RecoveryStateChanged.Broadcast(this, true);
}

void ARPGCreature::ExitRecovery()
{
	InRecovery = false;
	RPGEventManager->RecoveryStateChanged.Broadcast(this, false);
}