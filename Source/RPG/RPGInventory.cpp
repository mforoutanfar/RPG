// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGInventory.h"
#include "RPGInventoryItem.h"

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

bool URPGInventory::AddItem(FRPGItemInfo ItemInfo)
{
	int Width = ItemInfo.Width;
	int Height = ItemInfo.Height;

	for (size_t i = 0; i < Cols - Width + 1; i++)//col
	{
		for (size_t j = 0; j < Rows - Height + 1; j++)//row
		{
			if (DoesItemFit(Width, Height, j, i))
			{
				URPGInventoryItem* Item = CreateDefaultSubobject<URPGInventoryItem>("Item");
				Item->ItemInformation = ItemInfo;
				Items.Add(Item);

				for (size_t m = 0; m < Width; m++)//col
				{
					for (size_t n = 0; n < Height; n++)//row
					{
						Occupied[i + n][j + m] = true;
					}
				}

				return true;
			}
		}
	}

	return false;
}

bool URPGInventory::DoesItemFit(int Width, int Height, int Row, int Col)
{
	for (size_t m = 0; m < Width; m++)//col
	{
		for (size_t n = 0; n < Height; n++)//row
		{
			if (Col + m >= Cols || Row + n >= Rows || Occupied[Col + n][Row + m])
			{
				return false;
			}
		}
	}

	return true;
}


