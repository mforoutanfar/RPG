// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGRandomAudioComponent.h"

void URPGRandomAudioComponent::PlayRandom(FString BaseFileName)
{
	PopulateSoundsForKey(BaseFileName);

	int Range = SoundMap[BaseFileName].Num();

	if (Range != 0)
	{
		int rand = FMath::RandRange(0, Range-1);

		SetSound(SoundMap[BaseFileName][rand]);
		Play();
	}
}

TMap<FString, TArray<USoundBase*>> URPGRandomAudioComponent::SoundMap;

void URPGRandomAudioComponent::PopulateSoundsForKey(FString Key)
{
	if (!SoundMap.Contains(Key))
	{		
		FString BasePathToLoad = FString("/Game/Assets/Sounds/");

		SoundMap.Add(Key, TArray<USoundBase*>());

		int c = 0;
		while (true)
		{
			FString Number = FString::Printf(TEXT("%d"), c);
			FString FullPath = BasePathToLoad + Key + Number + "." + Key + Number;
			USoundBase* RSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, *(FullPath)));
			if (RSound)
			{
				SoundMap[Key].Add(RSound);
				c++;
			}
			else
			{
				break;
			}
		}
	}
}
