// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGFunctionLibrary.h"

#include "GameFramework/GameStateBase.h"
#include "RPG_GameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPG_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	ARPG_GameStateBase();

public:
	UPROPERTY(BlueprintReadOnly)
		class URPG_EventManager* EventManager = nullptr;

	UPROPERTY(BlueprintReadOnly)
		class ARPG_SafeAreaManager* SafeAreaManager = nullptr;

	TMap<FString, TArray<USoundBase*>> SoundMap;

	void PopulateSoundsForKey(FString Key);

	//TODO: Move recipe to seperate class?
	FName GetResultingItem(FName Ingredient1, FName Ingredient2);

	UPROPERTY(BlueprintReadOnly)
		class ARPG_SpawnManager* SpawnManager = nullptr;	

protected:
	virtual void OnConstruction(const FTransform& Transform);

	TMap<FName, TArray<FName>> Recepies;
};
