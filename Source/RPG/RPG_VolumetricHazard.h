// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFunctionLibrary.h"

#include "RPG_VolumetricHazard.generated.h"

/**
 *
*/
UCLASS()
class RPG_API ARPG_VolumetricHazard : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * Sets default values for this actor's properties
	*/
	ARPG_VolumetricHazard();

protected:
	/**
	 * Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Collider = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FRPGDice DamagePerSecond;

	UPROPERTY(EditDefaultsOnly)
	float DamageInterval = 0.0f;

	FTimerHandle DamageTimer;

	TSet<AActor*> OverlappingActors;

	void ApplyDamage();

public:	
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation 
	*/
	virtual void Tick(float DeltaTime) override;


};
