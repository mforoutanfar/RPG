// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG_InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_InventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	class RPGInventoryItem* RefItem = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URPG_ItemWidget> ItemClass;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* InventorySwitcher;

	UPROPERTY(meta = (BindWidget))
	class UImage* Background;

	void UpdateInventoryScale();

	void DoUpdateInventoryScale();

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnUnitAdded(class ARPGPlayerUnit* Unit);

	UFUNCTION()
	void OnSelectedUnitChanged(ARPGPlayerUnit* Unit);

	UFUNCTION()
	void OnInventoryItemAdded(class URPGInventoryItem* Item, class ARPGCreature* Creature);

	TMap <TWeakObjectPtr<class ARPGPlayerUnit>, class UCanvasPanel* > CanvasMap;
};
