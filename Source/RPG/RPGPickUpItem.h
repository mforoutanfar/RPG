// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFunctionLibrary.h"
#include  "RPG\RPGInteractable.h"
#include "RPG_Minimappable.h"

#include "RPGPickUpItem.generated.h"

using namespace ItemCategory;

/**
 *
*/
UCLASS()
class RPG_API ARPGPickUpItem : public AActor, public IRPGInteractable, public IRPG_Minimappable
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

public:	
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation 
	*/
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteracted(bool Successful) override;

	virtual bool IsInteractable() override;

	virtual InteractableCat GetInteractableType() override;

private:
	UPROPERTY(EditDefaultsOnly)
	class UBillboardComponent* ItemPicture;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* HitBox;
};
