// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPG_EventManager.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_EventManager : public UObject
{
	GENERATED_BODY()

private:
	static URPG_EventManager* Instance;

public:
	static URPG_EventManager* GetInstance();
};
