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

//k1*Rand(0,Dn) + k1
USTRUCT(BlueprintType)
struct FRPGDice
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		int k1;

	UPROPERTY(EditAnywhere)
		int Dn;

	UPROPERTY(EditAnywhere)
		int k2;

	int GetResult()
	{
		return k1 * FMath::RandRange(0, Dn) + k2;
	};
};

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

	UPROPERTY(EditAnywhere)
	FRPGDice MeleeDamage;

	UPROPERTY(EditAnywhere)
	float CriticalChance;

	UPROPERTY(EditAnywhere)
	float CriticalMultiplier;

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARPG_Projectile> ProjectileClass = nullptr;
};

USTRUCT(BlueprintType)
struct FRPGAttackData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		int Damage = 0;
	
	UPROPERTY(BlueprintReadWrite)
		float Accuracy = 0;

	UPROPERTY(BlueprintReadWrite)
		AActor* Attacker = nullptr;

	UPROPERTY(BlueprintReadWrite)
		AActor* Target = nullptr;

	UPROPERTY(BlueprintReadWrite)
		bool Ranged = false;
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
		bool Missed = false;

	UPROPERTY(BlueprintReadWrite)
		bool Crit = false;
};