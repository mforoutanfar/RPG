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
	UFUNCTION(BlueprintCallable)
	void PlayRandom(FString BaseFileName);

	FTimerHandle WalkingHandle;

	void SetWalkingSoundPlaying(bool Playing, float Interval);

protected:
	void PlayWalkSound();
};
