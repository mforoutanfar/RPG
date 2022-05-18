// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPGFunctionLibrary.h"

#include "RPG_EventManager.generated.h"

class ARPGPlayer;
class ARPGCreature;
class ARPGPlayerUnit;
class IRPGAttackable;
class URPGInventoryItem;
class URPG_ItemWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnitAdded, ARPGPlayerUnit*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRecoveryStateChanged, AActor*, Unit, bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackOccured, AActor*, Attacker, AActor*, Target, FRPGAttackResults, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedUnitChanged, ARPGPlayerUnit*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItemAdded, URPGInventoryItem*, Item, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemWidgetPicked, URPG_ItemWidget*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSafetyStateChanged, ARPGPlayerUnit*, Unit, TEnumAsByte<UnitSafety::SafetyState>, State);


/**
 * 
 */
UCLASS()
class RPG_API URPG_EventManager : public UObject
{
	GENERATED_BODY()

private:	
	static URPG_EventManager* Instance;

protected:
	virtual void BeginDestroy() override;

public:
	UFUNCTION(BlueprintCallable)
	static URPG_EventManager* GetInstance();

	UPROPERTY(BlueprintAssignable)
	FUnitAdded UnitAdded;

	UPROPERTY(BlueprintAssignable)
	FAttackOccured AttackOccured;

	UPROPERTY(BlueprintAssignable)
	FRecoveryStateChanged RecoveryStateChanged;

	UPROPERTY(BlueprintAssignable)
	FSelectedUnitChanged SelectedUnitChanged;

	UPROPERTY(BlueprintAssignable)
	FInventoryItemAdded InventoryItemAdded;

	UPROPERTY(BlueprintAssignable)
	FItemWidgetPicked ItemWidgetPicked;

	UPROPERTY(BlueprintAssignable)
	FSafetyStateChanged SafetyStateChanged;
};
