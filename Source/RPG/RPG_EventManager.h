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
class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnitAdded, ARPGPlayerUnit*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRecoveryStateChanged, AActor*, Unit, bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttackOccured, AActor*, Attacker, FRPGAttackData, AttackData, FRPGAttackResults, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInteractionOccured, AActor*, Interactor, TEnumAsByte<InteractableCategory::InteractableCat>, Category, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedUnitChanged, ARPGPlayerUnit*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItemAdded, URPGInventoryItem*, Item, AActor*, Owner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItemRemoved, URPGInventoryItem*, Item, AActor*, Owner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemWidgetClicked, URPG_ItemWidget*, Item, FName, ButtonName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemWidgetPicked, URPG_ItemWidget*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSafetyStateChanged, ARPGPlayerUnit*, Unit, TEnumAsByte<UnitSafety::SafetyState>, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAvatarClicked, AActor*, Actor, FName, ButtonName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAddItemToInventoryProposed, AActor*, Owner, FRPGItemInfo, ItemInfo, int, ProposedRow, int, ProposedCol);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddItemToEquipmentProposed, ARPGCreature*, Creature, FRPGItemInfo, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRemoveItemProposed, AActor*, Owner, URPGInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipmentItemAdded, URPGInventoryItem*, Item, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipmentItemRemoved, URPGInventoryItem*, Item, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentItemReplaced, FRPGItemInfo, PreviousItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNearestInteractableChanged, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FConsumeItemProposed, FRPGItemInfo, ItemInfo, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRemovePickedItemProposed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreatureStateChanged, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellCast, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverIssued);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreatureDied, ARPGCreature*, Creature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoinChanged, int, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoryEventTriggered, FString, EventName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddUnitProposed, TSubclassOf<ARPGPlayerUnit>, UnitClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHostileStateChanged, bool, Clear);
//TODO: Can we use AvatarClicked instead?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FContainerFocusStateChanged, AActor*, Container, bool, Open);
//
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FContainerClosed, AActor*, Container);

/**
 * 
 */
UCLASS(BlueprintType)
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
	FAvatarClicked AvatarClicked;

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

	UPROPERTY(BlueprintAssignable)
		FRemoveItemProposed RemoveItemProposed;

	UPROPERTY(BlueprintAssignable)
		FConsumeItemProposed ConsumeItemProposed;
	
	UPROPERTY(BlueprintAssignable)
		FRemovePickedItemProposed RemovePickedItemProposed;

	UPROPERTY(BlueprintAssignable)
		FCreatureStateChanged CreatureStateChanged;
	
	UPROPERTY(BlueprintAssignable)
		FSpellCast SpellCast;

	UPROPERTY(BlueprintAssignable)
		FGameOverIssued GameOverIssued;

	UPROPERTY(BlueprintAssignable)
		FCreatureDied CreatureDied;

	UPROPERTY(BlueprintAssignable)
		FCoinChanged CoinChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FStoryEventTriggered StoryEventTriggered;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FAddUnitProposed AddUnitProposed;

	UPROPERTY(BlueprintAssignable)
		FInteractionOccured InteractionOccured;

	UPROPERTY(BlueprintAssignable)
		FHostileStateChanged HostileStateChanged;

	UPROPERTY(BlueprintAssignable)
		FContainerFocusStateChanged ContainerFocusStateChanged;
	
	UPROPERTY(BlueprintAssignable)
		FContainerClosed ContainerClosed;
};
