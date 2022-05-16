// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPG_HUD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPG_HUD : public AHUD
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		TSubclassOf <class URPG_GameHUD> GameHUDClass;

	class URPG_GameHUD* GameHUD = nullptr;

public:
	void OnOpenInventoryPressed(bool InventoryOpen);
};
