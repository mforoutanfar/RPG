// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGFunctionLibrary.generated.h"

class ARPGCreature;

/**
 * 
 */
UCLASS()
class RPG_API URPGFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};

UENUM(BlueprintType)
namespace AnimationState
{
	enum AnimState
	{
		NONE,
		IDLE,
		WALK,
		ATTACK,
		HIT,
		DIE
	};
}

UENUM(BlueprintType)
namespace InteractableCategory
{
	enum InteractableCat
	{
		NONE,
		ITEM,
		DOOR,
		CHEST,
		CORPSE,
		MISC
	};
}

UENUM(BlueprintType)
namespace ItemCategory
{
	enum ItemCat
	{
		NONE,
		WEAPON,
		ARMOR,
		CONSUMABLE,
		INGREDIENTS,
		QUEST,
		MISC
	};
}

UENUM(BlueprintType)
namespace UnitSafety
{
	enum SafetyState
	{
		SAFE,
		WARNING,
		DANGER
	};
}

USTRUCT(BlueprintType)
struct FRPGItemInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FName ItemName = "";

	UPROPERTY()
	int Width = 0;

	UPROPERTY()
	int Height = 0;

	UPROPERTY()
	int InventoryX = -1;

	UPROPERTY()
	int InventoryY = -1;

	UPROPERTY()
	TEnumAsByte<ItemCategory::ItemCat> ItemCategory = ItemCategory::ItemCat::NONE;

	UPROPERTY()
	int Price = 0;
};

USTRUCT()
struct FRPGAttackData
{
	GENERATED_BODY()
public:
	UPROPERTY()
		int PhysicalDamage = 0;

	UPROPERTY()
		AActor* Attacker = nullptr;

	UPROPERTY()
		AActor* Target = nullptr;
};

USTRUCT(BlueprintType)
struct FRPGAttackResults
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		bool TargetDied = false;

	UPROPERTY(BlueprintReadWrite)
		float DamageDealt = 0.0f;

	UPROPERTY(BlueprintReadWrite)
		TWeakObjectPtr<AActor> Target = nullptr;
};