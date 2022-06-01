// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_EquipmentWidget.h"
#include "RPG_HUD.h"
#include "RPG_GameHUD.h"
#include "RPG_GameStateBase.h"
#include "RPG_EventManager.h"
#include "RPG_ItemWidget.h"
#include "RPG_Equipment.h"

void URPG_EquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EquipmentMap.Add(ItemCategory::ItemCat::MELEE_WEAPON, nullptr);
	EquipmentMap.Add(ItemCategory::ItemCat::RANGED_WEAPON, nullptr);
	EquipmentMap.Add(ItemCategory::ItemCat::ARMOR, nullptr);
}

FReply URPG_EquipmentWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (RPGGameHUD->PickedItem)
	{		
		RPGEventManager->AddItemToEquipmentProposed.Broadcast(EquipmentRef->OwnerUnit, RPGGameHUD->PickedItem->ItemInfo);
	}

	return FReply::Handled();
}
