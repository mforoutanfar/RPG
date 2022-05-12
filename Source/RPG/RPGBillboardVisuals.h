// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "RPGFunctionLibrary.h"

#include "RPGBillboardVisuals.generated.h"

using namespace AnimationState;

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

//enum AnimState
//{
//	NONE,
//	IDLE,
//	WALK,
//	ATTACK,
//	HIT,
//	DIE
//};

/**
 *
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class RPG_API URPGBillboardVisuals : public UBillboardComponent
{
	GENERATED_BODY()

	URPGBillboardVisuals();

	TMap<AnimState, TMap<Orientation, TArray<UTexture2D*>>> Sprites = {};
	TMap<AnimState, bool> ShouldLoopByDefault = {};
	TMap<AnimState, bool> ShouldReturnToDefault = {};
	TMap<AnimState, float> AnimSPF= {};

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOwnerWalkingStateChanged(ARPGCreature* Creature, bool State);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PopulateSprites();
	void UpdateOrientation();

	AActor* BillboardOwner;
	APlayerCameraManager* Camera;

	Orientation CurrentOrientation = BACK;
	AnimState CurrentAnimState = AnimState::NONE;
	AnimState DefaultAnimState = AnimState::IDLE;
	void SetDefaultAnimState(AnimState state);

	int CurrentFrame = 0;
	FTimerHandle TimerHandle;
	void AdvanceFrame();
	void UpdateSprite();

public:
	void SetAnimState(AnimState state);

	void Init(FString texturePrefix);

	UPROPERTY(EditAnywhere)
		FString TexturePrefix = "";

	void OnOwnerAttacked();
	void OnOwnerDied();
};
