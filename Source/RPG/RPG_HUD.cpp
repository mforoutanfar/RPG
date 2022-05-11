// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_HUD.h"
#include "RPG_GameHUD.h"
#include "Kismet/GameplayStatics.h"

void ARPG_HUD::BeginPlay()
{
	Super::BeginPlay();

	auto GameHUD = CreateWidget<URPG_GameHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0), GameHUDClass);
	GameHUD->AddToViewport();
}
