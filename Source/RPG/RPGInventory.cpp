// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGInventory.h"
#include "RPGInventoryItem.h"
#include "RPG_EventManager.h"
#include "RPGCreature.h"
#include "RPG_GameStateBase.h"
#include "Components/ScaleBox.h"

// Sets default values for this component's properties
URPGInventory::URPGInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void URPGInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void URPGInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool URPGInventory::Contains(URPGInventoryItem* Item)
{
	return Items.Contains(Item);
}

bool URPGInventory::AddItem(FRPGItemInfo& OutItemInfo, int ProposedRow, int ProposedCol)
{
	int Width = OutItemInfo.Width;
	int Height = OutItemInfo.Height;
	int DestRow = ProposedRow;
	int DestCol = ProposedCol;

	if (!DoesItemFit(Width, Height, DestRow, DestCol))
	{
		bool Found = false;
		for (size_t i = 0; i < Cols - Width + 1; i++)//col
		{
			for (size_t j = 0; j < Rows - Height + 1; j++)//row
			{
				if (DoesItemFit(Width, Height, j, i))
				{
					DestRow = j;
					DestCol = i;
					Found = true;
					break;
				}
			}

			if (Found)
			{
				break;
			}
		}

		if (!Found)
		{
			return false;
		}
	}

	OutItemInfo.InventoryX = DestCol;
	OutItemInfo.InventoryY = DestRow;
	OutItemInfo.Owner = Cast<AActor>(OwnerUnit);
	URPGInventoryItem* Item = NewObject<URPGInventoryItem>(this);
	Item->ItemInformation = OutItemInfo;
	Items.Add(Item);

	for (size_t m = 0; m < Width; m++)//col
	{
		for (size_t n = 0; n < Height; n++)//row
		{
			Occupied[DestRow + n][DestCol + m] = true;
		}
	}

	RPGEventManager->InventoryItemAdded.Broadcast(Item, OwnerUnit);

	RPGEventManager->RemovePickedItemProposed.Broadcast();

	return true;
}

void URPGInventory::RemoveItem(URPGInventoryItem* Item)
{
	int X = Item->ItemInformation.InventoryX;
	int Y = Item->ItemInformation.InventoryY;
	int Width = Item->ItemInformation.Width;
	int Height = Item->ItemInformation.Height;

	for (size_t m = X; m < X + Width; m++)//col
	{
		for (size_t n = Y; n < Y + Height; n++)//row
		{
			Occupied[n][m] = false;
		}
	}

	Item->ItemInformation.InventoryX = -1;
	Item->ItemInformation.InventoryY = -1;

	RPGEventManager->InventoryItemRemoved.Broadcast(Item, OwnerUnit);

	Item->ItemInformation.Owner = nullptr;

	Items.Remove(Item);
}

bool URPGInventory::DoesItemFit(int Width, int Height, int Row, int Col)
{
	for (size_t m = 0; m < Width; m++)//col
	{
		for (size_t n = 0; n < Height; n++)//row
		{
			if (Col + m >= Cols || Row + n >= Rows || Occupied[Row + n][Col + m])
			{
				return false;
			}
		}
	}

	return true;
}

TArray<URPGInventoryItem*> URPGInventory::GetItems()
{
	return Items;
}


