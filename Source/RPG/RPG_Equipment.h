// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGFunctionLibrary.h"
#include "RPG_Equipment.generated.h"

class URPGInventoryItem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API URPG_Equipment : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URPG_Equipment();

	bool AddItem(FRPGItemInfo OutItemInfo);

	class ARPGCreature* OwnerUnit = nullptr;

	URPGInventoryItem* GetItem(TEnumAsByte<ItemCategory::ItemCat> Category);

	void SetItem(TEnumAsByte<ItemCategory::ItemCat> Category, URPGInventoryItem* Item);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TMap<TEnumAsByte<ItemCategory::ItemCat>, URPGInventoryItem*> EquipmentMap;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool Contains(URPGInventoryItem* Item);

	void RemoveItem(URPGInventoryItem* Item);

};
