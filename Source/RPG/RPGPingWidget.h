// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGPingWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGPingWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UImage* PingImage;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* DefaultTexture;
	
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* PointerTexture;

public:
	void ChangeToDefault();
	void ChangeToPointer();
};
