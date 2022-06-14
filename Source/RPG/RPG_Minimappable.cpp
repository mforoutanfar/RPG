// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Minimappable.h"
#include "RPG_MiniMapWidget.h"
#include "RPG_HUD.h"
#include "RPG_GameHUD.h"
#include "RPGFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Add default functionality here for any IRPG_Minimappable functions that are not pure virtual.

void IRPG_Minimappable::RegisterOnMiniMap(AActor* Actor, MiniMap::ObjectType Type)
{
	if (auto PC = UGameplayStatics::GetPlayerController(Actor, 0))
	{
		if (auto RPGHUD = Cast<ARPG_HUD>(PC->GetHUD()))
		{
			if (auto GameHUD = RPGHUD->GameHUD)
			{
				GameHUD->RegisterOnMinimap(Actor, Type);
			}
		}
	}
}

void IRPG_Minimappable::UnregisterFromMiniMap(AActor* Actor)
{
	//TODO: When closing game, Player Controller is destroyed before some other actors (e.g. items and creatures). Solution?
	if (auto PC = UGameplayStatics::GetPlayerController(Actor, 0))
	{
		if (auto RPGHUD = Cast<ARPG_HUD>(PC->GetHUD()))
		{
			if (auto GameHUD = RPGHUD->GameHUD)
			{
				GameHUD->UnregisterFromMinimap(Actor);
			}
		}
	}
}
