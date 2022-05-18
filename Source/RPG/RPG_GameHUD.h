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
	TSubclassOf<class URPG_FollowerWidget> SlashWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URPG_DamageNumberWidget> DamageNumberWidgetClass;

	void OnOpenInventoryPressed(bool InventoryOpen);

	float InventoryScale = -1.0f;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnItemWidgetPicked(class URPG_ItemWidget* ItemWidget);

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* AvatarsBox;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Inventory;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Blur;

	UFUNCTION()
	void OnUnitAdded(ARPGPlayerUnit* Unit);

	UFUNCTION()
	void OnAttackOccured(AActor* Attacker, AActor* Target, FRPGAttackResults Results);
};
