// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include  "RPG\RPGInteractable.h"
#include "RPGPickUpItem.generated.h"

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

protected:
	/**
	 * Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

public:	
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation 
	*/
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteracted() override;

};
