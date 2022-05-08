// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGFunctionLibrary.generated.h"

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
	TEnumAsByte<ItemCategory::ItemCat> ItemCategory = ItemCategory::ItemCat::NONE;

	UPROPERTY()
	int Price = 0;
};