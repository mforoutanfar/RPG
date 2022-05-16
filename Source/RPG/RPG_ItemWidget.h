// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	void FollowMouse();

	void Init(URPGInventoryItem* ItemRef);

	void UpdateSizeForInventory();

	void UpdateSizeForHUD();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	static float InventoryScale;

protected:
	virtual void NativeConstruct() override;

	URPGInventoryItem* ItemRef = nullptr;	

	bool bIsSelected = false;

	FVector2D InitOffset;
	
};
