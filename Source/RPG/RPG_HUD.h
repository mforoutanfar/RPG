// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGFunctionLibrary.h"
#include "GameFramework/HUD.h"
#include "RPG_HUD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPG_HUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		TSubclassOf <class URPG_GameHUD> GameHUDClass;

public:
	void OnOpenInventoryPressed(bool InventoryOpen);

	void RegisterOnMinimap(AActor* Actor, TEnumAsByte<MiniMap::ObjectType> Type);
	void UnregisterFromMinimap(AActor* Actor);

	class URPG_GameHUD* GameHUD = nullptr;
};
