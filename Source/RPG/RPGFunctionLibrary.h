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
#define RPGGameState Cast<ARPG_GameStateBase>(GetWorld()->GetGameState())
#define RPGSafeAreaManager Cast<ARPG_GameStateBase>(GetWorld()->GetGameState())->SafeAreaManager

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
		AST_NONE,
		AST_IDLE,
		AST_WALK,
		AST_ATTACK,
		AST_HIT,
		AST_DIE
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
		LOCKED_DOOR,
		CHEST,
		LOCKED_CHEST,
		CORPSE,
		BUTTON,
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
		COIN,
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
	float HP = 0.0f;

	UPROPERTY(EditAnywhere)
	float Mana = 0.0f;

	UPROPERTY(EditAnywhere)
		float Armor = 0.0f;

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

	FString GetSpritePath()
	{
		FString ModifName = "";
		for (auto c : ItemName.ToString())
		{
			if (c == ' ')
			{
				continue;
			}

			ModifName += c;
		}

		FString AssetName = "Item_" + ModifName;
		FString PathToLoad = FString("/Game/Assets/Items/") + AssetName + FString(".") + AssetName;
		return PathToLoad;
	};
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

USTRUCT(BlueprintType)
struct FRPGLevelCombination
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		int NumberOfMelees = 0;

	UPROPERTY(EditAnywhere)
		int NumberOfRanged = 0;
};

USTRUCT(BlueprintType)
struct FRPGLootChance
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARPGPickUpItem> ItemClass = nullptr;

	UPROPERTY(EditAnywhere)
		float Chance = 1.0f;
};