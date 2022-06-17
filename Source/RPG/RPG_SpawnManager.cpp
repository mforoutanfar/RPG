// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_SpawnManager.h"
#include "RPGFunctionLibrary.h"
#include "RPG_GameStateBase.h"
#include "RPGUnit.h"
#include "RPGPickupItem.h"
#include "NavigationSystem.h"
#include "RPG_EventManager.h"

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

	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FActorSpawnParameters Params = FActorSpawnParameters();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (auto i : InitialLoot)
	{
		for (size_t j = 0; j < i.Value; j++)
		{
			auto Pos = navSystem->GetRandomPointInNavigableRadius(this, FVector::ZeroVector, Range);
			auto Actor = GetWorld()->SpawnActor<ARPGPickUpItem>(i.Key, Pos, FRotator::ZeroRotator, Params);
		}
	}
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPG_SpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPG_SpawnManager::SpawnNewWave()
{
	CurrentLevel++;

	if (LevelCombinationMap.Contains(CurrentLevel))
	{
		int NumberOfMelee = LevelCombinationMap[CurrentLevel].NumberOfMelees;
		int NumberOfRanged = LevelCombinationMap[CurrentLevel].NumberOfRanged;

		UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		FActorSpawnParameters Params = FActorSpawnParameters();
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		for (size_t i = 0; i < NumberOfMelee; i++)
		{
			float RadiusSquared = 0.0f;

			int Tries = 100;
			FVector Pos;

			//Don't spawn too close to player
			while (RadiusSquared < 2500.0f* 2500.0f)
			{
				Pos = navSystem->GetRandomReachablePointInRadius(this, FVector::ZeroVector, Range);
				RadiusSquared = Pos.SizeSquared2D();
				Tries--;
				if (Tries == 0)
				{
					break;
				}
			}
			
			auto Actor = GetWorld()->SpawnActor<ARPGUnit>(MeleeClass, Pos, FRotator::ZeroRotator, Params);
			SpawnedActors.Add(Actor);
		}

		for (size_t i = 0; i < NumberOfRanged; i++)
		{
			auto Pos = navSystem->GetRandomReachablePointInRadius(this, FVector::ZeroVector, Range);
			auto Actor = GetWorld()->SpawnActor<ARPGUnit>(RangedClass, Pos, FRotator::ZeroRotator, Params);
			SpawnedActors.Add(Actor);
		}
	}

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