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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnForwardBackwardPressed(float Value);
	void OnStrafePressed(float Value);
	void OnLookYaw(float Value);
	void OnLookPitch(float Value);
	void OnJumpPressed();
	void OnJumpReleased();
	void OnRunPressed();
	void OnRunReleased();
	void OnInteractPressed();
	void OnAttackPressed();
	void OnSwitchUnitPressed();

	bool CanGenerallyInteractWithTarget(IRPGInteractable* Target);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	void AddUnit();

	const int UnitCapacity = 4;

	UPROPERTY(EditDefaultsOnly)
	int StarterUnits = 1;

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* PlayerCameraComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* InteractionCollider;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ARPGPlayerUnit> UnitClass;

	class URPGRandomAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* SightSource;

	TArray<ARPGPlayerUnit*> Units = {};

	UPROPERTY()
	class ASceneCapture2D* MiniMapCamera = nullptr;

	AActor* GetNearestTarget(class UShapeComponent* Collider, bool ShouldBeVisible = true);

	UFUNCTION()
	void OnUnitRecoveryStateChanged(AActor* Unit, bool IsInRecovery);

	ARPGPlayerUnit* FindFirstOutOfRecoveryUnit();

	void SetSelectedUnit(ARPGPlayerUnit* Unit);

	TWeakObjectPtr<ARPGPlayerUnit> SelectedUnit = nullptr;
};
