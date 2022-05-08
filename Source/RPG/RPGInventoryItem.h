// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPGFunctionLibrary.h"

#include "RPGInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
	FRPGItemInfo ItemInformation;

public:
	//Location of Top Left square of Item in Inventory
	//int PosX = 0;
	//int PosY = 0;
};
