// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "RPGBillboardVisuals.generated.h"

enum Orientation
{
	BACK,
	BACK_RIGHT,
	RIGHT,
	FRONT_RIGHT,
	FRONT,
	FRONT_LEFT,
	LEFT,
	BACK_LEFT
};

/**
 *
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class RPG_API URPGBillboardVisuals : public UBillboardComponent
{
	GENERATED_BODY()

	URPGBillboardVisuals();

	UPROPERTY(EditDefaultsOnly)
		FString TexturePrefix = "TestChar";

	TMap<Orientation, TArray<UTexture2D*>> WalkSprites = {};
	TMap<Orientation, TArray<UTexture2D*>> IdleSprites = {};

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PopulateSprites();
	void UpdateOrientation();

	AActor* BillboardOwner;
	APlayerCameraManager* Camera;

	Orientation CurrentOrientation = FRONT;
};
