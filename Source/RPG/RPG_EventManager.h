// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPG_EventManager.generated.h"

class ARPGPlayer;
class ARPGPlayerUnit;

DECLARE_DELEGATE_OneParam(FUnitAdded, ARPGPlayerUnit*);

/**
 * 
 */
UCLASS()
class RPG_API URPG_EventManager : public UObject
{
	GENERATED_BODY()

private:	
	static TWeakObjectPtr<URPG_EventManager> Instance;

public:
	UFUNCTION()
	static TWeakObjectPtr<URPG_EventManager> GetInstance();

	FUnitAdded UnitAdded;
};
