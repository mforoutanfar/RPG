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
