// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "RPGPlayer.generated.h"

class ARPGPlayerUnit;
class IRPGInteractable;

UCLASS(Blueprintable/*, HideCategories = (Character)*/)
class RPG_API ARPGPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGPlayer();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	void AddUnit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const int UnitCapacity = 4;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnForwardBackwardPressed(float Value);
	void OnStrafePressed(float Value);
	void OnLookYaw(float Value);
	void OnLookPitch(float Value);
	void OnJumpPressed();
	void OnJumpReleased();
	void OnRunPressed();
	void OnRunReleased();
	void OnInteractPressed();
	bool CanGenerallyInteractWithTarget(IRPGInteractable* Target);
	void OnAttackPressed();

	void OnSwitchUnitPressed();

private:

	TArray<class UChildActorComponent*> UnitPlaceHolders = {};

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* PlayerCameraComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* InteractionCollider;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* MeleeBox;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* RangedSphere;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ARPGPlayerUnit> UnitClass;

	class URPGRandomAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* SightSource;

	TArray<ARPGPlayerUnit*> Units = {};

	AActor* GetNearestTarget(class UShapeComponent* Collider, bool ShouldBeVisible = true);

	void OnUnitRecoveryStateChanged(TWeakObjectPtr<ARPGPlayerUnit> Unit, bool IsInRecovery);

	ARPGPlayerUnit* FindFirstOutOfRecoveryUnit();

	void SetSelectedUnit(ARPGPlayerUnit* Unit);

	TWeakObjectPtr<ARPGPlayerUnit> SelectedUnit = nullptr;
};
