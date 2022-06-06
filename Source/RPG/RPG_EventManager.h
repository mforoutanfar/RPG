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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackOccured, AActor*, Attacker, FRPGAttackData, AttackData, FRPGAttackResults, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedUnitChanged, ARPGPlayerUnit*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItemAdded, URPGInventoryItem*, Item, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItemRemoved, URPGInventoryItem*, Item, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemWidgetClicked, URPG_ItemWidget*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemWidgetPicked, URPG_ItemWidget*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSafetyStateChanged, ARPGPlayerUnit*, Unit, TEnumAsByte<UnitSafety::SafetyState>, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAvatarLeftClicked, ARPGPlayerUnit*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAvatarRightClicked, ARPGPlayerUnit*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAddItemToInventoryProposed, ARPGCreature*, Creature, FRPGItemInfo, ItemInfo, int, ProposedRow, int, ProposedCol);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddItemToEquipmentProposed, ARPGCreature*, Creature, FRPGItemInfo, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipmentItemAdded, URPGInventoryItem*, Item, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipmentItemRemoved, URPGInventoryItem*, Item, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentItemReplaced, FRPGItemInfo, PreviousItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNearestInteractableChanged, AActor*, Actor);


/**
 * 
 */
UCLASS()
class RPG_API URPG_EventManager : public UObject
{
	GENERATED_BODY()

public:
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
	FInventoryItemRemoved InventoryItemRemoved;

	UPROPERTY(BlueprintAssignable)
	FItemWidgetClicked ItemWidgetClicked;

	UPROPERTY(BlueprintAssignable)
	FItemWidgetPicked ItemWidgetPicked;

	UPROPERTY(BlueprintAssignable)
	FSafetyStateChanged SafetyStateChanged;

	UPROPERTY(BlueprintAssignable)
	FAvatarLeftClicked AvatarLeftClicked;

	UPROPERTY(BlueprintAssignable)
	FAvatarRightClicked AvatarRightClicked;

	UPROPERTY(BlueprintAssignable)
	FAddItemToInventoryProposed AddItemToInventoryProposed;

	UPROPERTY(BlueprintAssignable)
		FAddItemToEquipmentProposed AddItemToEquipmentProposed;

	UPROPERTY(BlueprintAssignable)
		FEquipmentItemAdded EquipmentItemAdded;

	UPROPERTY(BlueprintAssignable)
		FEquipmentItemRemoved EquipmentItemRemoved;

	UPROPERTY(BlueprintAssignable)
		FEquipmentItemReplaced EquipmentItemReplaced;

	UPROPERTY(BlueprintAssignable)
		FNearestInteractableChanged NearestInteractableChanged;
};
