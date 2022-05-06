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

enum AnimState
{
	IDLE,
	WALK
};

/**
 *
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class RPG_API URPGBillboardVisuals : public UBillboardComponent
{
	GENERATED_BODY()

	URPGBillboardVisuals();

	TMap<AnimState, TMap<Orientation, TArray<UTexture2D*>>> Sprites = {};

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PopulateSprites();
	void UpdateOrientation();

	AActor* BillboardOwner;
	APlayerCameraManager* Camera;

	Orientation CurrentOrientation = BACK;
	AnimState CurrentAnimState = IDLE;
	int CurrentFrame = 0;
	float spf = 0.2f;	
	FTimerHandle TimerHandle;
	void AdvanceFrame();
	void UpdateSprite();

public:
	void SetAnimState(AnimState state);

	UPROPERTY(EditAnywhere)
		FString TexturePrefix = "TestChar";

};
