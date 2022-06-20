// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Container.h"
#include "RPGInventory.h"
#include "RPGPickupItem.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"
#include "RPGInventoryItem.h"
#include "RPG_ItemWidget.h"

/**
 * Sets default values
*/
ARPG_Container::ARPG_Container()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Inventory = CreateDefaultSubobject<URPGInventory>(FName("Inventory"));
	Inventory->OwnerUnit = this;
}

/**
 * Called when the game starts or when spawned
*/
void ARPG_Container::BeginPlay()
{
	Super::BeginPlay();	

	RPGEventManager->ItemWidgetPicked.AddDynamic(this, &ARPG_Container::OnItemWidgetPicked);
	RPGEventManager->RemoveItemProposed.AddDynamic(this, &ARPG_Container::OnRemoveItemProposed);
	RPGEventManager->AddItemToInventoryProposed.AddDynamic(this, &ARPG_Container::OnAddItemToInventoryProposed);
	RPGEventManager->ContainerClosed.AddDynamic(this, &ARPG_Container::OnContainerClosed);

	//Populate Item Name Map.
	for (auto i : StartingInventoryItems)
	{
		auto Item = NewObject<ARPGPickUpItem>(this, i.Key);
		ItemNameMap.Add(Item->ItemInformation.ItemName, i.Key);
		Item->Destroy();
	}
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPG_Container::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPG_Container::OnContainerClosed(AActor* Container)
{
	for (auto i : Inventory->GetItems())
	{
		Inventory->RemoveItem(i);
	}
}

void ARPG_Container::OnItemWidgetPicked(URPG_ItemWidget* Item)
{
	if (auto ref = Item->ItemRef)
	{
		if (Inventory->Contains(ref))
		{
			Inventory->RemoveItem(ref);

			auto PickUpItemClass = ItemNameMap[Item->ItemInfo.ItemName];
			StartingInventoryItems[PickUpItemClass] = StartingInventoryItems[PickUpItemClass]-1;
			if (StartingInventoryItems[PickUpItemClass] == 0)
			{
				StartingInventoryItems.Remove(PickUpItemClass);
			}
		}
	}
}

void ARPG_Container::OnRemoveItemProposed(AActor* InvOwner, URPGInventoryItem* Item)
{
	if (InvOwner == this)
	{
		if (Inventory->Contains(Item))
		{
			Inventory->RemoveItem(Item);

			auto PickUpItemClass = ItemNameMap[Item->ItemInformation.ItemName];
			StartingInventoryItems[PickUpItemClass] = StartingInventoryItems[PickUpItemClass] - 1;
			if (StartingInventoryItems[PickUpItemClass] == 0)
			{
				StartingInventoryItems.Remove(PickUpItemClass);
			}
		}
	}
}

void ARPG_Container::OnAddItemToInventoryProposed(AActor* InvOwner, FRPGItemInfo ItemInfo, int ProposedRow, int ProposedCol)
{
	if (InvOwner == this)
	{
		Inventory->AddItem(ItemInfo, ProposedRow, ProposedCol);

		auto PickUpItemClass = ItemNameMap[ItemInfo.ItemName];
		if (StartingInventoryItems.Contains(PickUpItemClass))
		{
			StartingInventoryItems[PickUpItemClass] = StartingInventoryItems[PickUpItemClass] + 1;
		}
		else
		{
			StartingInventoryItems.Add(PickUpItemClass, 1);
		}
	}
}

void ARPG_Container::Open()
{
	RPGEventManager->StoryEventTriggered.Broadcast("Container");

	RPGEventManager->AvatarClicked.Broadcast(nullptr, "LeftMouseButton");

	//Leads to creation of respective InventoryWidget.
	RPGEventManager->ContainerFocusStateChanged.Broadcast(this, true);

	for (auto i : StartingInventoryItems)
	{
		auto Item = NewObject<ARPGPickUpItem>(this, i.Key);
		Item->SetRandomPrice();

		for (size_t j = 0; j < i.Value; j++)
		{
			//Leads to creation of item widgets in inventory widget.
			Inventory->AddItem(Item->ItemInformation, FMath::RandRange(0,11), FMath::RandRange(0, 11));
		}

		Item->Destroy();
	}
}
