// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFunctionLibrary.h"

#include "RPG_Projectile.generated.h"

/**
 *
*/
UCLASS()
class RPG_API ARPG_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * Sets default values for this actor's properties
	*/
	ARPG_Projectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditDefaultsOnly)
	float Accuracy = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float BaseCriticalChance = 0.01f;

	UPROPERTY(EditDefaultsOnly)
	float BaseCriticalMultiplier = 2.0f;
	
	UPROPERTY(EditDefaultsOnly)
	FRPGDice BaseMeleeDamage;

protected:
	/**
	 * Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* MovementComponent = nullptr;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent = nullptr;

	void CalculateDamage(FRPGAttackData& OutData, FRPGAttackResults& Results);

	/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
	virtual void Tick(float DeltaTime) override;

public:	

};
