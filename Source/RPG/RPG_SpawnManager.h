// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFunctionLibrary.h"
#include "RPG_SpawnManager.generated.h"

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

	int CurrentLevel = -1;

public:
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
	*/
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnNewWave();

	UPROPERTY(EditAnywhere)
		TMap<int, FRPGLevelCombination> LevelCombinationMap;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ARPGUnit> MeleeClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
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
};