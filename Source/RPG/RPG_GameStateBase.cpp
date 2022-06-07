// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameStateBase.h"
#include "RPG_EventManager.h"

ARPG_GameStateBase::ARPG_GameStateBase()
{
	EventManager = CreateDefaultSubobject<URPG_EventManager>(FName("EventManager"));
}

void ARPG_GameStateBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ARPG_GameStateBase::PopulateSoundsForKey(FString Key)
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
