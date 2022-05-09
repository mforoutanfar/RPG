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

};
