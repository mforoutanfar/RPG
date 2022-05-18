// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RPG_GameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPG_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	ARPG_GameStateBase();

public:
	UPROPERTY(BlueprintReadWrite)
		class URPG_EventManager* EventManager = nullptr;

protected:
	virtual void OnConstruction(const FTransform& Transform);

};
