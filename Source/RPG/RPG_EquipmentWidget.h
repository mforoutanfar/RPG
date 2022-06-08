// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGFunctionLibrary.h"
#include "RPG_EquipmentWidget.generated.h"

class URPG_ItemWidget;
class ARPGPlayerUnit;
class UWidgetSwitcher;
class USizeBox;
class URPGInventoryItem;
class ARPGCreature;

/**
 *
 */
UCLASS()
class RPG_API URPG_EquipmentWidget : public UUserWidget
{
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPG_ItemWidget> ItemClass;

protected:
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TMap<TEnumAsByte<ItemCategory::ItemCat>, UWidgetSwitcher*> SwitcherMap;

	UPROPERTY(meta = (BindWidget))
		USizeBox* MeleeSizeBox = nullptr;

	UPROPERTY(meta = (BindWidget))
		USizeBox* RangedSizeBox = nullptr;

	UPROPERTY(meta = (BindWidget))
		USizeBox* ArmorSizeBox = nullptr;

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* MeleeSwitcher;

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* RangedSwitcher;

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* ArmorSwitcher;

	bool HasUpdatedSize = false;

	bool HasInit = false;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	TMap<TEnumAsByte<ItemCategory::ItemCat>, TMap <TWeakObjectPtr<ARPGPlayerUnit>, UWidget*>> EquipmentMap;	

	UFUNCTION()
		void OnUnitAdded(ARPGPlayerUnit* Unit);

	UFUNCTION()
		void OnSelectedUnitChanged(ARPGPlayerUnit* Unit);

	UFUNCTION()
		void OnEquipmentItemAdded(URPGInventoryItem* Item, ARPGCreature* Creature);

	UFUNCTION()
		void OnEquipmentItemRemoved(URPGInventoryItem* Item, ARPGCreature* Creature);

	UFUNCTION()
		void OnItemWidgetClicked(URPG_ItemWidget* ItemWidget, FName ButtonName);

	bool IsItemWidgetInEquipmentWidget(URPG_ItemWidget* ItemWidget);

	TWeakObjectPtr<ARPGPlayerUnit> SelectedUnit = nullptr;

	URPG_ItemWidget* ClickedItemWidget = nullptr;
};
