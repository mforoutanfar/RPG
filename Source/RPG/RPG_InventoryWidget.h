// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG_InventoryWidget.generated.h"

class URPG_ItemWidget;

/**
 * 
 */
UCLASS()
class RPG_API URPG_InventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPG_ItemWidget> ItemClass;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* InventorySwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Background;

	//void UpdateInventoryScale();

	//void DoUpdateInventoryScale();

protected:

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnItemWidgetClicked(URPG_ItemWidget* ItemWidget, FName ButtonName);

	bool IsItemWidgetInInventoryWidget(URPG_ItemWidget* ItemWidget);

	UFUNCTION()
	void OnUnitAdded(class ARPGPlayerUnit* Unit);

	UFUNCTION()
	void OnSelectedUnitChanged(ARPGPlayerUnit* Unit);

	UFUNCTION()
	void OnInventoryItemAdded(class URPGInventoryItem* Item, class ARPGCreature* Creature);

	TMap <TWeakObjectPtr<class ARPGPlayerUnit>, class UCanvasPanel* > CanvasMap;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//TODO: Link to RPGInventory class
	const int Rows = 12;
	const int Cols = 12;

	URPG_ItemWidget* LeftClickedItemWidget = nullptr;
	URPG_ItemWidget* RightClickedItemWidget = nullptr;
};
