// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_HUD.h"
#include "RPG_GameHUD.h"
#include "Kismet/GameplayStatics.h"

void ARPG_HUD::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//Put here instead of BeginPlay because otherwise registeronminimap won't work. TODO: Find solution.
	GameHUD = CreateWidget<URPG_GameHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0), GameHUDClass);
	GameHUD->AddToViewport();
}

void ARPG_HUD::BeginPlay()
{
	Super::BeginPlay();
}

void ARPG_HUD::OnOpenInventoryPressed(bool InventoryOpen)
{
	GameHUD->OnOpenInventoryPressed(InventoryOpen);
}

void ARPG_HUD::OnPausePressed(bool PauseMenuOpen)
{
	GameHUD->OnPausePressed(PauseMenuOpen);
}

void ARPG_HUD::RegisterOnMinimap(AActor* Actor, TEnumAsByte<MiniMap::ObjectType> Type)
{
	GameHUD->RegisterOnMinimap(Actor, Type);
}

void ARPG_HUD::UnregisterFromMinimap(AActor* Actor)
{
	GameHUD->UnregisterFromMinimap(Actor);
}
