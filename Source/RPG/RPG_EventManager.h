// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPGFunctionLibrary.h"

#include "RPG_EventManager.generated.h"

class ARPGPlayer;
class ARPGCreature;
class ARPGPlayerUnit;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnitAdded, ARPGPlayerUnit*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCreatureWalkingStateChanged, ARPGCreature*, Creature, bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRecoveryStateChanged, AActor*, Unit, bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUnitAttackedEnemy, ARPGPlayerUnit*, Unit, FRPGAttackResults, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEnemyAttackedUnit, ARPGPlayerUnit*, Unit, FRPGAttackResults, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedUnitChanged, ARPGPlayerUnit*, Unit);

/**
 * 
 */
UCLASS()
class RPG_API URPG_EventManager : public UObject
{
	GENERATED_BODY()

private:	
	static TWeakObjectPtr<URPG_EventManager> Instance;

protected:
	virtual void BeginDestroy() override;

public:
	UFUNCTION(BlueprintCallable)
	static URPG_EventManager* GetInstance();

	UPROPERTY(BlueprintAssignable)
	FUnitAdded UnitAdded;

	UPROPERTY(BlueprintAssignable)
	FUnitAttackedEnemy UnitAttackedEnemy;

	UPROPERTY(BlueprintAssignable)
	FEnemyAttackedUnit EnemyAttackedUnit;

	UPROPERTY(BlueprintAssignable)
	FRecoveryStateChanged RecoveryStateChanged;

	UPROPERTY(BlueprintAssignable)
	FSelectedUnitChanged SelectedUnitChanged;

	UPROPERTY(BlueprintAssignable)
	FCreatureWalkingStateChanged CreatureWalkingStateChanged;
};
