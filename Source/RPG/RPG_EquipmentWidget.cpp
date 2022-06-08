// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_EquipmentWidget.h"
#include "RPG_HUD.h"
#include "RPG_GameHUD.h"
#include "RPG_GameStateBase.h"
#include "RPG_EventManager.h"
#include "RPG_ItemWidget.h"
#include "RPG_Equipment.h"
#include "RPGPlayerUnit.h"
#include "RPGInventoryItem.h"
#include "Blueprint/WidgetTree.h"

#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WidgetSwitcherSlot.h"

void URPG_EquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TMap <TWeakObjectPtr<ARPGPlayerUnit>, UWidget*> MeleeMap;
	TMap <TWeakObjectPtr<ARPGPlayerUnit>, UWidget*> RangedMap;
	TMap <TWeakObjectPtr<ARPGPlayerUnit>, UWidget*> ArmorMap;

	EquipmentMap.Add(ItemCategory::ItemCat::MELEE_WEAPON, MeleeMap);
	EquipmentMap.Add(ItemCategory::ItemCat::RANGED_WEAPON, RangedMap);
	EquipmentMap.Add(ItemCategory::ItemCat::ARMOR, ArmorMap);

	SwitcherMap.Add(ItemCategory::ItemCat::MELEE_WEAPON, MeleeSwitcher);
	SwitcherMap.Add(ItemCategory::ItemCat::RANGED_WEAPON, RangedSwitcher);
	SwitcherMap.Add(ItemCategory::ItemCat::ARMOR, ArmorSwitcher);

	RPGEventManager->UnitAdded.AddDynamic(this, &URPG_EquipmentWidget::OnUnitAdded);
	RPGEventManager->EquipmentItemAdded.AddDynamic(this, &URPG_EquipmentWidget::OnEquipmentItemAdded);
	RPGEventManager->EquipmentItemRemoved.AddDynamic(this, &URPG_EquipmentWidget::OnEquipmentItemRemoved);
	RPGEventManager->SelectedUnitChanged.AddDynamic(this, &URPG_EquipmentWidget::OnSelectedUnitChanged);
	RPGEventManager->ItemWidgetClicked.AddDynamic(this, &URPG_EquipmentWidget::OnItemWidgetClicked);
}

void URPG_EquipmentWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	if (!HasUpdatedSize)
	{
		auto Sc = URPG_ItemWidget::InventoryScale;

		MeleeSizeBox->SetWidthOverride(Sc * MeleeSizeBox->WidthOverride);
		MeleeSizeBox->SetHeightOverride(Sc * MeleeSizeBox->HeightOverride);

		RangedSizeBox->SetWidthOverride(Sc * RangedSizeBox->WidthOverride);
		RangedSizeBox->SetHeightOverride(Sc * RangedSizeBox->HeightOverride);

		ArmorSizeBox->SetWidthOverride(Sc * ArmorSizeBox->WidthOverride);
		ArmorSizeBox->SetHeightOverride(Sc * ArmorSizeBox->HeightOverride);

		HasUpdatedSize = true;
	}
}

FReply URPG_EquipmentWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (RPGGameHUD->PickedItem)
	{
		RPGEventManager->AddItemToEquipmentProposed.Broadcast(SelectedUnit.Get(), RPGGameHUD->PickedItem->ItemInfo);
	}
	else if (ClickedItemWidget)
	{
		RPGEventManager->ItemWidgetPicked.Broadcast(ClickedItemWidget);	
	}

	ClickedItemWidget = nullptr;

	return FReply::Handled();
}

void URPG_EquipmentWidget::OnUnitAdded(ARPGPlayerUnit* Unit)
{
	int UnitIndex = Unit->UnitIndex;
	TWeakObjectPtr<ARPGPlayerUnit> WeakPtr = Unit;

	for (auto &i: EquipmentMap)
	{
		i.Value.Add(WeakPtr, nullptr);
	}
}

void URPG_EquipmentWidget::OnSelectedUnitChanged(ARPGPlayerUnit* Unit)
{
	SelectedUnit = Unit;

	if (Unit)//It is possible that no unit is selected.
	{
		for (auto i : EquipmentMap)
		{
			if (i.Value[Unit])
			{
				SwitcherMap[i.Key]->SetVisibility(ESlateVisibility::Visible);
				SwitcherMap[i.Key]->SetActiveWidget(i.Value[Unit]);
			}
			else
			{
				SwitcherMap[i.Key]->SetVisibility(ESlateVisibility::Collapsed);//Turning visibility off because we can't switch to a null widget.
			}
		}
	}
}

void URPG_EquipmentWidget::OnEquipmentItemAdded(URPGInventoryItem* Item, ARPGCreature* Creature)
{
	auto ItemWidget = CreateWidget<URPG_ItemWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ItemClass);
	auto PlayerUnit = Cast<ARPGPlayerUnit>(Creature);

	auto Cat = Item->ItemInformation.ItemCategory;

	SwitcherMap[Cat]->AddChild(ItemWidget);
	EquipmentMap[Cat][PlayerUnit] = ItemWidget;
	SwitcherMap[Cat]->SetVisibility(ESlateVisibility::Visible);
	SwitcherMap[Cat]->SetActiveWidget(ItemWidget);

	auto WSSlot = Cast<UWidgetSwitcherSlot>(ItemWidget->Slot);

	WSSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	WSSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

	ItemWidget->Init(Item, Item->ItemInformation);

	Item->RefWidget = ItemWidget;
}

void URPG_EquipmentWidget::OnEquipmentItemRemoved(URPGInventoryItem* Item, ARPGCreature* Creature)
{
	auto Player = Cast<ARPGPlayerUnit>(Creature);

	auto Cat = Item->ItemInformation.ItemCategory;

	EquipmentMap[Cat][Player]->RemoveFromParent();
	EquipmentMap[Cat][Player] = nullptr;

	//Because switcher will switch to next available item.
	SwitcherMap[Cat]->SetVisibility(ESlateVisibility::Collapsed);
}

void URPG_EquipmentWidget::OnItemWidgetClicked(URPG_ItemWidget* ItemWidget, FName ButtonName)
{
	if (ButtonName == "LeftMouseButton")
	{
		if (IsItemWidgetInEquipmentWidget(ItemWidget))//Make sure it's not in inventory widget. TODO: Looks too complicated. Solution?
		{
			ClickedItemWidget = ItemWidget;
		}
	}
	else if (ButtonName == "RightMouseButton")
	{

	}
}

bool URPG_EquipmentWidget::IsItemWidgetInEquipmentWidget(URPG_ItemWidget* ItemWidget)
{	
	auto Cat = ItemWidget->ItemInfo.ItemCategory;

	if (EquipmentMap.Contains(Cat))
	{
		for (auto i : EquipmentMap[Cat])
		{
			if (i.Value == ItemWidget)
			{
				return true;
			}
		}
	}

	return false;
}
