// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGFunctionLibrary.h"

#include "RPGInventory.generated.h"

class URPGInventoryItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API URPGInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGInventory();

	bool AddItem(FRPGItemInfo ItemInfo);

	bool DoesItemFit(int width, int height, int row, int col);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	static const int Rows = 20;
	static const int Cols = 30;

	//Top Left is {Row = 0,Col = 0}.
	bool Occupied[Rows][Cols] = {false};

	UPROPERTY()
	TArray<URPGInventoryItem*> Items = {};

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
