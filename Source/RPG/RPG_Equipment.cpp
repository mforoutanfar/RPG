// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Equipment.h"
#include "RPG_GameStateBase.h"
#include "RPG_EventManager.h"
#include "RPGInventoryItem.h"
#include "RPGCreature.h"

// Sets default values for this component's properties
URPG_Equipment::URPG_Equipment()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	EquipmentMap.Add(ItemCategory::ItemCat::MELEE_WEAPON, nullptr);
	EquipmentMap.Add(ItemCategory::ItemCat::RANGED_WEAPON, nullptr);
	EquipmentMap.Add(ItemCategory::ItemCat::ARMOR, nullptr);

	// ...
}

bool URPG_Equipment::AddItem(FRPGItemInfo OutItemInfo)
{
	if (EquipmentMap.Contains(OutItemInfo.ItemCategory))
	{
		//Remove current item.
		auto PreviousItem = EquipmentMap[OutItemInfo.ItemCategory];
		bool PreviousItemExists = false;
		FRPGItemInfo PreviousInfo;
		if (PreviousItem)
		{
			PreviousItemExists = true;
			PreviousInfo = PreviousItem->ItemInformation;
			RemoveItem(PreviousItem);
		}

		//Add new item. (leads to removal of picked item)
		OutItemInfo.Owner = Cast<AActor>(OwnerUnit);
		URPGInventoryItem* Item = NewObject<URPGInventoryItem>(this);
		Item->ItemInformation = OutItemInfo;

		EquipmentMap[OutItemInfo.ItemCategory] = Item;

		RPGEventManager->EquipmentItemAdded.Broadcast(Item, OwnerUnit);

		//Add previous item as picked item.
		if (PreviousItemExists)
		{
			RPGEventManager->EquipmentItemReplaced.Broadcast(PreviousInfo);
		}
		return true;
	}
	else
	{
		//TODO: Play error sound?
		return false;
	}
}


URPGInventoryItem* URPG_Equipment::GetItem(TEnumAsByte<ItemCategory::ItemCat> Category)
{
	if (EquipmentMap.Contains(Category))
	{
		return EquipmentMap[Category];
	}
	else
	{
		return nullptr;
	}
}

void URPG_Equipment::SetItem(TEnumAsByte<ItemCategory::ItemCat> Category, URPGInventoryItem* Item)
{
	if (EquipmentMap.Contains(Category))
	{
		EquipmentMap[Category] = Item;
	}
}

// Called when the game starts
void URPG_Equipment::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void URPG_Equipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool URPG_Equipment::Contains(URPGInventoryItem* Item)
{
	//TODO: Might have multiple equipment of same category in the future.
	if (EquipmentMap.Contains(Item->ItemInformation.ItemCategory))
	{
		return (EquipmentMap[Item->ItemInformation.ItemCategory] == Item);
	}

	return false;
}

void URPG_Equipment::RemoveItem(URPGInventoryItem* Item)
{
	Item->ItemInformation.Owner = nullptr;
	RPGEventManager->EquipmentItemRemoved.Broadcast(Item, OwnerUnit);
	EquipmentMap[Item->ItemInformation.ItemCategory] = nullptr;
	Item->ConditionalBeginDestroy();
}
