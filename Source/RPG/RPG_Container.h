// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFunctionLibrary.h"

#include "RPG_Container.generated.h"

/**
 *
*/

//TODO: Unify with creature inventory system?
UCLASS()
class RPG_API ARPG_Container : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * Sets default values for this actor's properties
	*/
	ARPG_Container();

protected:
	/**
	 * Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

	TMap<FName, TSubclassOf<class ARPGPickUpItem>> ItemNameMap;

public:	
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation 
	*/
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnContainerClosed(AActor* Container);

	UFUNCTION()
	void OnItemWidgetPicked(class URPG_ItemWidget* Item);

	UFUNCTION()
	void OnRemoveItemProposed(AActor* InvOwner, class URPGInventoryItem* Item);

	UFUNCTION()
	void OnAddItemToInventoryProposed(AActor* InvOwner, FRPGItemInfo ItemInfo, int ProposedRow, int ProposedCol);

	class URPGInventory* Inventory;

	UPROPERTY(EditAnywhere)
		TMap<TSubclassOf<class ARPGPickUpItem>, int> StartingInventoryItems;

	UFUNCTION(BlueprintCallable)
	void Open();
};
