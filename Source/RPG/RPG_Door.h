// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include  "RPG\RPGInteractable.h"

#include "RPG_Door.generated.h"

/**
 *
*/
UCLASS()
class RPG_API ARPG_Door : public AActor, public IRPGInteractable
{
	GENERATED_BODY()
	
public:	
	/**
	 * Sets default values for this actor's properties
	*/
	ARPG_Door();

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

	void OnInteracted_Implementation(bool Successful) override;

	bool IsInteractable_Implementation() override;

	InteractableCat GetInteractableType_Implementation() override;

	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* HitBox;

	UPROPERTY(EditAnywhere)
		FString EventName = "";
};
