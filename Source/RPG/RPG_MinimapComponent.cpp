// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_MinimapComponent.h"
#include "RPG_MiniMapWidget.h"
#include "RPG_HUD.h"
#include "RPG_GameHUD.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
URPG_MinimapComponent::URPG_MinimapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URPG_MinimapComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URPG_MinimapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URPG_MinimapComponent::RegisterOnMiniMap(MiniMap::ObjectType Type)
{
	if (auto PC = UGameplayStatics::GetPlayerController(GetOwner(), 0))
	{
		if (auto RPGHUD = Cast<ARPG_HUD>(PC->GetHUD()))
		{
			if (auto GameHUD = RPGHUD->GameHUD)
			{
				GameHUD->RegisterOnMinimap(GetOwner(), Type);
			}
		}
	}
}

void URPG_MinimapComponent::UnregisterFromMiniMap()
{
	//TODO: When closing game, Player Controller is destroyed before some other actors (e.g. items and creatures). Solution?
	if (auto PC = UGameplayStatics::GetPlayerController(GetOwner(), 0))
	{
		if (auto RPGHUD = Cast<ARPG_HUD>(PC->GetHUD()))
		{
			if (auto GameHUD = RPGHUD->GameHUD)
			{
				GameHUD->UnregisterFromMinimap(GetOwner());
			}
		}
	}
}
