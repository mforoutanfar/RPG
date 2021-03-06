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
#include "RPG_Spell.h"
#include "RPGPickUpItem.h"

// Sets default values
ARPGCreature::ARPGCreature()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->DestroyComponent();
	GetMesh()->SetActive(false);

	MeleeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeSphere"));
	MeleeSphere->ComponentTags.Add(FName("MeleeSphere"));
	MeleeSphere->SetupAttachment(RootComponent);

	RangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
	RangeSphere->ComponentTags.Add(FName("RangeSphere"));
	RangeSphere->SetupAttachment(RootComponent);

	HitBox = GetCapsuleComponent();

	AudioComponent = CreateDefaultSubobject<URPGRandomAudioComponent>(FName("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	//So that it will play when paused
	AudioComponent->SetTickableWhenPaused(true);
	AudioComponent->bIsUISound = true;
	//

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
	RPGEventManager->RemoveItemProposed.AddDynamic(this, &ARPGCreature::OnRemoveItemProposed);
	RPGEventManager->ConsumeItemProposed.AddDynamic(this, &ARPGCreature::OnConsumeItemProposed);

	for (auto i : SpellClassess)
	{
		auto Spell = NewObject<URPG_Spell>(this, i);
		Spell->Caster = this;
		Spells.Add(Spell);
	}
}

void ARPGCreature::AddStartingInventoryItems()
{
	for (auto i : StartingInventoryItems)
	{
		auto Rand = FMath::FRandRange(0.0f, 1.0f);

		if (Rand > i.Chance)
		{
			continue;
		}

		//TODO: Better solution for storing item data and creating items.
		auto Item = NewObject<ARPGPickUpItem>(this, i.ItemClass);
		Inventory->AddItem(Item->ItemInformation);
		Item->Destroy();
	}
}

void ARPGCreature::OnConsumeItemProposed(FRPGItemInfo ItemInfo, ARPGCreature* Creature)
{
	if (Creature == this)
	{
		if (IsDead())
		{
			return;
		}

		HP += ItemInfo.HP;
		if (HP > MaxHP)
		{
			HP = MaxHP;
		}

		Mana += ItemInfo.Mana;
		if (Mana > MaxMana)
		{
			Mana = MaxMana;
		}

		RPGEventManager->CreatureStateChanged.Broadcast(this);

		RPGEventManager->RemovePickedItemProposed.Broadcast();

		AudioComponent->PlayRandom("gulp");
	}
}

//TODO: Move all inventory related functions to RPGInventory?
void ARPGCreature::OnRemoveItemProposed(AActor* InvOwner, URPGInventoryItem* Item)
{
	if (InvOwner == this)
	{
		if (Inventory->Contains(Item))
		{
			Inventory->RemoveItem(Item);
		}
	}
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

void ARPGCreature::OnAttacked_Implementation(FRPGAttackData& AttackData, FRPGAttackResults& Results)
{
	AttackData.Target = this;

	int DamageDealt = AttackData.Damage;

	auto Armor = BaseArmor;

	if (auto ArmorItem = Equipment->GetItem(ItemCategory::ItemCat::ARMOR))
	{
		Armor = ArmorItem->ItemInformation.Armor;
	}

	DamageDealt = DamageDealt * (1.0f - Armor/ 100.f);//TODO: Change armor system?

	auto rand = FMath::RandRange(0.0f,1.0f);

	float RecoveryDuration = 1.0f;

	if (rand > AttackData.Accuracy)//miss
	{
		RecoveryDuration = 0.0f;
		Results.Missed = true;
		DamageDealt = 0.0f;
	}
	else if (Results.Crit)
	{
		AudioComponent->PlayRandom("crit");
	}
	else
	{
		AudioComponent->PlayRandom("hit");
	}

	HP -= DamageDealt;

	Results.DamageDealt = DamageDealt;

	if (HP <= 0)
	{
		Die();
		Results.TargetDied = true;
		RecoveryDuration = 1.0f;
		EnterRecovery(RecoveryDuration);
		//Dead creature should never exit recovery.
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		return;
	}

	EnterRecovery(RecoveryDuration);
}


void ARPGCreature::BeginAttack()
{
	auto Results = Attack();
	EnterRecovery(Results.RecoveryDuration);
}

bool ARPGCreature::CastReadySpell()
{
	if (ReadySpellIndex < Spells.Num())
	{
		auto Spell = Spells[ReadySpellIndex];
		if (Mana > Spell->RequiredMana)
		{
			Spell->CastSpell();
			Mana -= Spell->RequiredMana;
			RPGEventManager->CreatureStateChanged.Broadcast(this);
			RPGEventManager->SpellCast.Broadcast(this);
			EnterRecovery(Spell->RecoveryDuration);

			return true;
		}
	}

	return false;
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
		AttackData.Attacker = this;
		AttackData.Target = NearestMelee;
		CalculateMeleeParams(AttackData, Results);

		auto Attackable = Cast<IRPGAttackable>(NearestMelee);
		Attackable->Execute_OnAttacked(NearestMelee, AttackData, Results);
	}
	else
	{
		if (auto NearestRanged = GetNearestAttackTarget(RangeSphere, ExcludeOwnType, ShouldBeVisible))
		{
			TSubclassOf<ARPG_Projectile> ProjClass = nullptr;

			if (auto RangedEq = Equipment->GetItem(ItemCategory::ItemCat::RANGED_WEAPON))
			{
				ProjClass = RangedEq->ItemInformation.ProjectileClass;
			}
			else if (ProjectileClass)
			{
				ProjClass = ProjectileClass;
			}

			if (ProjClass)
			{
				auto ModifActionLocation = GetActorRotation().RotateVector(ActionLocation);
				auto PointOfAction = GetActorLocation() + ModifActionLocation;
				auto Direction = (NearestRanged->GetActorLocation() - PointOfAction).Rotation();

				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				auto Projectile = GetWorld()->SpawnActor<ARPG_Projectile>(ProjClass, PointOfAction, Direction, Params);
				//TODO: Projectile is sometimes null for some reason.
				if (Projectile)
				{
					Projectile->Accuracy = FMath::Max(Projectile->Accuracy, BaseAccuracy);
					Projectile->BaseCriticalChance = FMath::Max(Projectile->BaseCriticalChance, BaseCriticalChance);
					Projectile->BaseCriticalMultiplier = FMath::Max(Projectile->BaseCriticalMultiplier, BaseCriticalMultiplier);

					AttackData.Ranged = true;
				}
			}
		}
	}

	if (AttackData.Ranged)
	{
		AudioComponent->PlayRandom("arrow_shot");
	}
	else
	{
		AudioComponent->PlayRandom("whoosh");
	}

	Results.RecoveryDuration = RecoveryDuration;

	RPGEventManager->AttackOccured.Broadcast(this, AttackData, Results);

	return Results;
}

//void ARPGCreature::ShootProjectile(TSubclassOf<ARPG_Projectile> ProjectileClass, )
//{
//
//}

void ARPGCreature::CalculateMeleeParams(FRPGAttackData& OutData, FRPGAttackResults& Results)
{
	if (auto Weapon = Equipment->GetItem(ItemCategory::ItemCat::MELEE_WEAPON))
	{
		auto Damage = Weapon->ItemInformation.MeleeDamage.GetResult();
		float rand = FMath::RandRange(0.0f, 1.0f);
		bool IsCrit = rand < FMath::Max(Weapon->ItemInformation.CriticalChance, BaseCriticalChance);
		if (IsCrit)
		{
			Damage *= Weapon->ItemInformation.CriticalMultiplier;
		}

		Results.Crit = IsCrit;

		OutData.Damage = Damage;
	}
	else
	{
		auto Damage = BaseMeleeDamage.GetResult();
		float rand = FMath::RandRange(0.0f, 1.0f);
		bool IsCrit = rand < BaseCriticalChance;
		if (IsCrit)
		{
			Damage *= BaseCriticalMultiplier;
		}

		Results.Crit = IsCrit;

		OutData.Damage = Damage;
	}

	OutData.Accuracy = BaseAccuracy;
}

void ARPGCreature::Die()
{
	Dead = true;
	RPGEventManager->CreatureDied.Broadcast(this);
}

bool ARPGCreature::IsAttackable_Implementation()
{
	return !Dead;
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

		if (!Actor->Implements<URPGAttackable>())
		{
			continue;
		}
		else
		{
			auto Attackable = Cast<IRPGAttackable>(Actor);

			if (!Attackable->Execute_IsAttackable(Actor))
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

			if (Creature->IsDead())
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
	if (Duration > 0)
	{
		GetWorldTimerManager().ClearTimer(RecoveryTimerHandle);

		InRecovery = true;

		GetWorldTimerManager().SetTimer(RecoveryTimerHandle, this, &ARPGCreature::ExitRecovery, Duration, false);
		RPGEventManager->RecoveryStateChanged.Broadcast(this, true);
	}
}

void ARPGCreature::ExitRecovery()
{
	InRecovery = false;
	RPGEventManager->RecoveryStateChanged.Broadcast(this, false);
}