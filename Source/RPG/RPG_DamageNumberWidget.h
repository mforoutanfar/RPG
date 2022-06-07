// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_FollowerWidget.h"
#include "RPGFunctionLibrary.h"
#include "RPG_DamageNumberWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_DamageNumberWidget : public URPG_FollowerWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	UWidget* Projectile;

	FVector2D Velocity;
	FVector2D Position;

	float Life;
	float Gravity;
	float ProjectileTimer = 0.0f;
	bool ProjectileAnimationRunning = false;

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void MakeProjectileAnimation(UWidget* InWidget, float InitVelocity, float InitAngle, float gravity, float Life);

	UPROPERTY(BlueprintReadOnly)
		FRPGAttackResults AttackResults;

public:
	void Init(UObject* InTarget, FRPGAttackResults InAttackResults);

};
