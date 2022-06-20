// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGRandomAudioComponent.h"
#include "RPG_GameStateBase.h"

void URPGRandomAudioComponent::PlayRandom(FString BaseFileName)
{
	//TODO: In DebugGame, GetWorld returns null sometimes.
	auto a = GetWorld();
	auto b = a->GetGameState();	
	auto GS = Cast<ARPG_GameStateBase>(b);

	GS->PopulateSoundsForKey(BaseFileName);

	int Range = GS->SoundMap[BaseFileName].Num();

	if (Range != 0)
	{
		int rand = FMath::RandRange(0, Range-1);
		auto SoundBase = GS->SoundMap[BaseFileName][rand];		
		SetSound(SoundBase);
		bIsUISound = true;
		Play();
	}
}

void URPGRandomAudioComponent::SetWalkingSoundPlaying(bool Playing, float Interval)
{
	GetWorld()->GetTimerManager().ClearTimer(WalkingHandle);

	if (Playing)
	{
		PlayWalkSound();
		GetWorld()->GetTimerManager().SetTimer(WalkingHandle, this, &URPGRandomAudioComponent::PlayWalkSound, Interval, true);
	}
}

void URPGRandomAudioComponent::PlayWalkSound()
{
	PlayRandom("walk");
}