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
namespace CreatureAnimState
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