// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGFunctionLibrary.h"
#include "RPG_EquipmentWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_EquipmentWidget : public UUserWidget
{
public:

protected:
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MeleePlaceholder = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidget* RangedPlaceholder = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidget* ArmorPlaceholder = nullptr;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
		
	class URPG_Equipment* EquipmentRef = nullptr;

	TMap<TEnumAsByte<ItemCategory::ItemCat>, class URPGInventoryItem*> EquipmentMap;
};
