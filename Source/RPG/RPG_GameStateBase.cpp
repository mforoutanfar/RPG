// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameStateBase.h"
#include "RPG_EventManager.h"

ARPG_GameStateBase::ARPG_GameStateBase()
{
	EventManager = CreateDefaultSubobject<URPG_EventManager>(FName("EventManager"));

	Recepies.Add("Health Potion", TArray<FName>({ "Bottle", "Berries" }));
	Recepies.Add("Mana Potion", TArray<FName>({ "Bottle", "Blueberries" }));
}

void ARPG_GameStateBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

FName ARPG_GameStateBase::GetResultingItem(FName Ingredient1, FName Ingredient2)
{
	for (auto i: Recepies)
	{
		if (i.Value.Contains(Ingredient1) && i.Value.Contains(Ingredient2))
		{
			return i.Key;
		}
	}

	return "";
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
