// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"

#include "RPGRandomAudioComponent.generated.h"

/**
 * 
 */

UCLASS()
class RPG_API URPGRandomAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	void PlayRandom(FString BaseFileName);

private:
	static TMap<FString, TArray<USoundBase*>> SoundMap;
    static void PopulateSoundsForKey(FString Key);
};
