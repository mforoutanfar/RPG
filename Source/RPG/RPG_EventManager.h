// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPGFunctionLibrary.h"

#include "RPG_EventManager.generated.h"

class ARPGPlayer;
class ARPGCreature;
class ARPGPlayerUnit;

DECLARE_MULTICAST_DELEGATE_OneParam(FUnitAdded, TWeakObjectPtr<ARPGPlayerUnit>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FCreatureWalkingStateChanged, TWeakObjectPtr<ARPGCreature>, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FRecoveryStateChanged, TWeakObjectPtr<ARPGPlayerUnit>, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUnitAttackedEnemy, TWeakObjectPtr<ARPGPlayerUnit>, FRPGAttackResults);
DECLARE_MULTICAST_DELEGATE_OneParam(FSelectedUnitChanged, TWeakObjectPtr<ARPGPlayerUnit>);

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
	FUnitAttackedEnemy UnitAttackedEnemy;
	FRecoveryStateChanged RecoveryStateChanged;
	FSelectedUnitChanged SelectedUnitChanged;
	FCreatureWalkingStateChanged CreatureWalkingStateChanged;
};
