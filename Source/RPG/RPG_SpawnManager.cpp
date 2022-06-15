// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_SpawnManager.h"
#include "RPGFunctionLibrary.h"
#include "RPG_GameStateBase.h"
#include "RPGUnit.h"
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

	SetLevelCleared(true);

	RPGEventManager->CreatureDied.AddDynamic(this, &ARPG_SpawnManager::OnCreatureDied);
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

		for (size_t i = 0; i < NumberOfMelee; i++)
		{
			UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			
			auto Pos = navSystem->GetRandomReachablePointInRadius(this, FVector::ZeroVector, 10000.f);
			FActorSpawnParameters Params = FActorSpawnParameters();
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			auto Actor = GetWorld()->SpawnActor<ARPGUnit>(MeleeClass, Pos, FRotator::ZeroRotator, Params);
			SpawnedActors.Add(Actor);
		}

		for (size_t i = 0; i < NumberOfRanged; i++)
		{
			UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());

			auto Pos = navSystem->GetRandomReachablePointInRadius(this, FVector::ZeroVector, 10000.f);
			FActorSpawnParameters Params = FActorSpawnParameters();
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			auto Actor = GetWorld()->SpawnActor<ARPGUnit>(RangedClass, Pos, FRotator::ZeroRotator, Params);
			SpawnedActors.Add(Actor);
		}
	}

	SetLevelCleared(false);
}

void ARPG_SpawnManager::OnCreatureDied(ARPGCreature* Unit)
{
	SpawnedActors.Remove(Unit);

	if (SpawnedActors.Num() == 0)
	{
		SetLevelCleared(true);
	}
}

void ARPG_SpawnManager::SetLevelCleared(bool Cleared)
{
	LevelCleared = Cleared;
}

