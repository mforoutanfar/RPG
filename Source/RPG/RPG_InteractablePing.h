// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_FollowerWidget.h"
#include "RPGFunctionLibrary.h"

#include "RPG_InteractablePing.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_InteractablePing : public URPG_FollowerWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPingString(InteractableCategory::InteractableCat Cat);
};
