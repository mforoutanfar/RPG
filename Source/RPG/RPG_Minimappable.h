// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGFunctionLibrary.h"

#include "RPG_Minimappable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URPG_Minimappable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IRPG_Minimappable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

protected:
	void RegisterOnMiniMap(AActor* Actor, MiniMap::ObjectType Type);
	void UnregisterFromMiniMap(AActor* Actor);
};
