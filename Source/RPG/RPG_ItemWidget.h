// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGFunctionLibrary.h"
#include "RPG_ItemWidget.generated.h"

class URPGInventoryItem;

/**
 * 
 */
UCLASS()
class RPG_API URPG_ItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void FollowMouse(bool HasLag);

	void Init(URPGInventoryItem* ItemRef, FRPGItemInfo InItemInfo);

	void UpdateSizeForInventory();

	void UpdateSizeForHUD();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	static float InventoryScale;

	URPGInventoryItem* ItemRef = nullptr;

	FRPGItemInfo ItemInfo;

	bool bShouldFollowMouse = false;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnItemRemoved(URPGInventoryItem* Item, class ARPGCreature* Creature);
};
