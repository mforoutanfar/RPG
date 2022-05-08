// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGPlayer.generated.h"

class ARPGPlayerUnit;

UCLASS(Blueprintable/*, HideCategories = (Character)*/)
class RPG_API ARPGPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGPlayer();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
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
	void OnInteractReleased();
	void OnAttackReleased();

private:

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

	TArray<ARPGPlayerUnit*> Units = {};

	AActor* GetNearestTarget(class UShapeComponent* Collider, bool ShouldBeVisible = true);

	void OnUnitRecoveryStateChanged(ARPGPlayerUnit* Unit, bool IsInRecovery);

	ARPGPlayerUnit* FindFirstOutOfRecoveryUnit();

	void SetActiveUnit(ARPGPlayerUnit* Unit);

	TWeakObjectPtr<ARPGPlayerUnit> ActiveUnit = nullptr;
};
