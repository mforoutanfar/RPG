// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "RPGFunctionLibrary.generated.h"

#define RPGEventManager Cast<ARPG_GameStateBase>(GetWorld()->GetGameState())->EventManager
#define RPGPlayerController Cast<ARPGPlayerController>(GetWorld()->GetFirstPlayerController())
#define RPGPlayer Cast<ARPGPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())
#define RPGGameHUD Cast<ARPG_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->GameHUD

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
namespace MiniMap
{
	enum ObjectType
	{
		ENEMY,
		LOOT,
		NPC
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
		MELEE_WEAPON,
		RANGED_WEAPON,
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

	UPROPERTY(EditAnywhere)
	FName ItemName = "";

	UPROPERTY(EditAnywhere)
	int Width = 0;

	UPROPERTY(EditAnywhere)
	int Height = 0;

	UPROPERTY()
	int InventoryX = -1;

	UPROPERTY()
	int InventoryY = -1;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ItemCategory::ItemCat> ItemCategory = ItemCategory::ItemCat::NONE;

	UPROPERTY(EditAnywhere)
	int Price = 0;

	UPROPERTY()
	AActor* Owner = nullptr;
};

USTRUCT()
struct FRPGAttackData
{
	GENERATED_BODY()
public:
	UPROPERTY()
		int Damage = 0;

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
		float RecoveryDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite)
		TWeakObjectPtr<AActor> Target = nullptr;

	//TODO: Hack for preventing slash animation showing on ranged attacks.
	UPROPERTY(BlueprintReadWrite)
		bool Ranged = false;
};