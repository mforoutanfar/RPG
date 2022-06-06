// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "RPGFunctionLibrary.h"

#include "RPGBillboardVisuals.generated.h"

using namespace AnimationState;

DECLARE_DELEGATE_RetVal(FRPGAttackResults, FAnimCallback);

enum Orientation
{
	OR_BACK,
	OR_BACK_RIGHT,
	OR_RIGHT,
	OR_FRONT_RIGHT,
	OR_FRONT,
	OR_FRONT_LEFT,
	OR_LEFT,
	OR_BACK_LEFT
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

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PopulateSprites();
	void UpdateOrientation();

	APlayerCameraManager* Camera;

	Orientation CurrentOrientation = OR_BACK;
	AnimState CurrentAnimState = AnimState::AST_NONE;
	AnimState DefaultAnimState = AnimState::AST_IDLE;
	void SetDefaultAnimState(AnimState state);

	int CurrentFrame = 0;
	FTimerHandle TimerHandle;
	void AdvanceFrame();
	void UpdateSprite();

	int AttackCallbackFrame = 5;

public:

	UFUNCTION()
		void OnOwnerWalkingStateChanged(ARPGCreature* Creature, bool State);

	void SetAnimState(AnimState state);

	void Init(FString texturePrefix);

	UPROPERTY(EditAnywhere)
		FString TexturePrefix = "";

	void OnOwnerDied();

	UFUNCTION()
	void OnAttackOccured(AActor* Attacker, FRPGAttackData Data, FRPGAttackResults Results);

	void BeginAttack();
	
	FAnimCallback AttackCallback;
};
