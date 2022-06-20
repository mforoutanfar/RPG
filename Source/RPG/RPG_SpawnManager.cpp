// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_SpawnManager.h"
#include "RPGFunctionLibrary.h"
#include "RPG_GameStateBase.h"
#include "RPGUnit.h"
#include "RPGPickupItem.h"
#include "NavigationSystem.h"
#include "RPG_EventManager.h"
#include "RPG_SafeAreaManager.h"

/**
 * Sets default values
*/
ARPG_SpawnManager::ARPG_SpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * Called when the game starts or when spawned
*/
void ARPG_SpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	RPGGameState->SpawnManager = this;

	RPGEventManager->CreatureDied.AddDynamic(this, &ARPG_SpawnManager::OnCreatureDied);
	RPGEventManager->CreatureExpired.AddDynamic(this, &ARPG_SpawnManager::OnCreatureExpired);

	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FActorSpawnParameters Params = FActorSpawnParameters();

	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (auto i : InitialLoot)
	{
		for (size_t j = 0; j < i.Value; j++)
		{
			FNavLocation Pos;
			do
			{
				navSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, MaxRange, Pos);
			} while (!RPGSafeAreaManager->IsPointInSafeArea(Pos));

			auto Actor = GetWorld()->SpawnActor<ARPGPickUpItem>(i.Key, Pos, FRotator::ZeroRotator, Params);
		}
	}

	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (size_t i = 0; i < MeleePoolSize; i++)
	{
		auto Actor = GetWorld()->SpawnActor<ARPGUnit>(MeleeClass, FVector(100.f*i, 100.f * i,-5000.f), FRotator::ZeroRotator, Params);
		AddUnitToPool(Actor, MeleePool);
	}

	for (size_t i = 0; i < RangedPoolSize; i++)
	{
		auto Actor = GetWorld()->SpawnActor<ARPGUnit>(RangedClass, FVector(-100.f * i, -100.f * i, -5000.f), FRotator::ZeroRotator, Params);
		AddUnitToPool(Actor, RangedPool);
	}

	int MaxEffects = 0;

	for (auto i : LevelCombinationMap)
	{
		int Tot = i.Value.NumberOfMelees + i.Value.NumberOfRanged;

		if (Tot > MaxEffects)
		{
			MaxEffects = Tot;
		}
	}
}

ARPGUnit* ARPG_SpawnManager::GetUnitFromPool(TArray<ARPGUnit*>& Pool)
{
	if (Pool.Num() > 0)
	{
		auto Actor = Pool.Last();
		Pool.Remove(Actor);
		return Actor;
	}
	else
	{
		return nullptr;
	}
}

void ARPG_SpawnManager::AddUnitToPool(ARPGUnit* Unit, TArray<ARPGUnit*>& Pool)
{
	Pool.Add(Unit);
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPG_SpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARPG_SpawnManager::SpawnUnitsFromPool(TArray<ARPGUnit*>& Pool, int Number)
{
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FActorSpawnParameters Params = FActorSpawnParameters();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (size_t i = 0; i < Number; i++)
	{
		float RadiusSquared = 0.0f;

		int Tries = 100;
		FNavLocation Pos;

		//Don't spawn too close to player		
		do
		{
			navSystem->GetRandomReachablePointInRadius(FVector::ZeroVector, MaxRange, Pos);
			RadiusSquared = ((FVector)Pos).SizeSquared2D();
			Tries--;
			if (Tries == 0)
			{
				break;
			}
		} while (RadiusSquared < MinRange * MinRange || !RPGSafeAreaManager->IsPointInSafeArea(Pos));

		if (auto Actor = GetUnitFromPool(Pool))
		{
			FVector AdjustedLocation = Pos;
			AdjustedLocation.Z += 100.f;
			FRotator AdjustedRotation = FRotator::ZeroRotator;
			if (GetWorld()->FindTeleportSpot(Actor, AdjustedLocation, AdjustedRotation))
			{
				Actor->SetActorLocationAndRotation(AdjustedLocation, AdjustedRotation, false, nullptr, ETeleportType::TeleportPhysics);
			}
			Actor->RPGSetActive(true);

			SpawnedActors.Add(Actor);
		}
	}
}

void ARPG_SpawnManager::SpawnNewWave()
{
	CurrentLevel++;

	FDateTime UTCTime1 = FDateTime::UtcNow();	

	if (LevelCombinationMap.Contains(CurrentLevel))
	{
		SpawnUnitsFromPool(MeleePool, LevelCombinationMap[CurrentLevel].NumberOfMelees);
		SpawnUnitsFromPool(RangedPool, LevelCombinationMap[CurrentLevel].NumberOfRanged);
	}

	FDateTime UTCTime2 = FDateTime::UtcNow();

	auto Delta = UTCTime2 - UTCTime1;
	auto Milli = Delta.GetSeconds() + Delta.GetTotalMilliseconds() / 1000.f;

	//GEngine->AddOnScreenDebugMessage(-1, 1000, FColor::Yellow, FString::Printf(TEXT("%f"), Milli));

	RPGEventManager->HostileStateChanged.Broadcast(false);
}

void ARPG_SpawnManager::OnCreatureDied(ARPGCreature* Unit)
{
	SpawnedActors.Remove(Unit);

	if (SpawnedActors.Num() == 0)
	{
		RPGEventManager->HostileStateChanged.Broadcast(true);
	}
}

void ARPG_SpawnManager::OnCreatureExpired(ARPGCreature* Creature)
{
	if (auto Unit = Cast<ARPGUnit>(Creature))
	{
		Unit->RPGSetActive(false);

		if (Unit->CreatureName == "Archer")
		{
			AddUnitToPool(Unit, RangedPool);
		}
		else
		{
			AddUnitToPool(Unit, MeleePool);
		}
	}
}
