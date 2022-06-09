// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Projectile.h"
#include "RPG_Fireball.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPG_Fireball : public ARPG_Projectile
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void PlayExplosionAnimation();

	ARPG_Fireball();

	UPROPERTY(EditAnywhere)
		class USphereComponent* ExplosionSphereComponent = nullptr;
		
	virtual void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnConstruction(const FTransform& Transform) override;

};
