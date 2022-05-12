// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGFunctionLibrary.h"

#include "RPG_GameHUD.generated.h"

class ARPGPlayerUnit;

/**
 * 
 */
UCLASS()
class RPG_API URPG_GameHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URPG_AvatarWidget> AvatarClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URPG_SlashWidget> DamageWidgetClass;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* AvatarsBox;

	UFUNCTION()
	void OnUnitAdded(ARPGPlayerUnit* Unit);

	UFUNCTION()
	void OnUnitAttackedEnemy(ARPGPlayerUnit* Unit, FRPGAttackResults Results);
};
