// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG_AvatarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_AvatarWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UImage* Portrait;

	UPROPERTY(meta = (BindWidget))
	class UImage* RecoveryIndicator;

	UPROPERTY(meta = (BindWidget))
	class UImage* ActiveIndicator;
};
