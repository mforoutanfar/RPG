// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFunctionLibrary.h"
#include "RPG_SpawnManager.generated.h"

class ARPGUnit;

/**
 *
*/
UCLASS()
class RPG_API ARPG_SpawnManager : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	*/
	ARPG_SpawnManager();

protected:
	/**
	 * Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

	ARPGUnit* GetUnitFromPool(TArray<ARPGUnit*>& Pool);

	void AddUnitToPool(ARPGUnit* Unit, TArray<ARPGUnit*>& Pool);

	int CurrentLevel = -1;

public:
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
	*/
	virtual void Tick(float DeltaTime) override;

	void SpawnUnitsFromPool(TArray<ARPGUnit*>& Pool, int Number);

	UFUNCTION(BlueprintCallable)
	void SpawnNewWave();

	UPROPERTY(EditAnywhere)
		TMap<int, FRPGLevelCombination> LevelCombinationMap;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARPGUnit> MeleeClass = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARPGUnit> RangedClass = nullptr;

	UPROPERTY(EditAnywhere)
		TMap<TSubclassOf<class ARPGPickUpItem>, int> InitialLoot;

	UPROPERTY(EditAnywhere)
		float MinRange = 4000.0f;

	UPROPERTY(EditAnywhere)
		float MaxRange = 4000.0f;

	TArray<AActor*> SpawnedActors;

	UFUNCTION()
		void OnCreatureDied(class ARPGCreature* Unit);

	UFUNCTION()
		void OnCreatureExpired(class ARPGCreature* Unit);

	UPROPERTY(EditAnywhere)
	int MeleePoolSize = 100;

	UPROPERTY(EditAnywhere)
	int RangedPoolSize = 100;

	TArray<ARPGUnit*> MeleePool;
	TArray<ARPGUnit*> RangedPool;
};