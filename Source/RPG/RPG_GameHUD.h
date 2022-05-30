// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGFunctionLibrary.h"

#include "RPG_GameHUD.generated.h"

class ARPGPlayerUnit;
class URPG_ItemWidget;

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

	void RegisterOnMinimap(AActor* Actor, TEnumAsByte<MiniMap::ObjectType> Type);
	void UnregisterFromMinimap(AActor* Actor);

	URPG_ItemWidget* PickedItem = nullptr;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class URPG_ItemWidget> ItemClass;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnItemWidgetPicked(URPG_ItemWidget* ItemWidget);

	UFUNCTION()
	void OnInventoryItemAdded(class URPGInventoryItem* Item, class ARPGCreature* Creature);

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* AvatarsBox;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Inventory;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Background;

	UPROPERTY(meta = (BindWidget))
	class URPG_MiniMapWidget* MiniMap;

	UFUNCTION()
	void OnUnitAdded(ARPGPlayerUnit* Unit);

	UFUNCTION()
	void OnAttackOccured(AActor* Attacker, AActor* Target, FRPGAttackResults Results);
};
