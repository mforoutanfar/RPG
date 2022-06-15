// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFunctionLibrary.h"
#include  "RPG\RPGInteractable.h"

#include "RPGPickUpItem.generated.h"

using namespace ItemCategory;

/**
 *
*/
UCLASS()
class RPG_API ARPGPickUpItem : public AActor, public IRPGInteractable
{
	GENERATED_BODY()
	
public:	
	/**
	 * Sets default values for this actor's properties
	*/
	ARPGPickUpItem();

	UPROPERTY(EditDefaultsOnly)
	FRPGItemInfo ItemInformation;

protected:
	/**
	 * Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	class URPG_MinimapComponent* MinimapComponent = nullptr;

public:	
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation 
	*/
	virtual void Tick(float DeltaTime) override;

	void OnInteracted_Implementation(bool Successful) override;

	bool IsInteractable_Implementation() override;

	InteractableCat GetInteractableType_Implementation() override;

private:
	UPROPERTY(EditDefaultsOnly)
	class UBillboardComponent* ItemPicture;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* HitBox;
};
