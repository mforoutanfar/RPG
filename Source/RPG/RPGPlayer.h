// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGFunctionLibrary.h"

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

	void UpdateWalkingSoundState();

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
	void OnSpellPressed();
	void OnSwitchUnitPressed();

	bool CanGenerallyInteractWithTarget(IRPGInteractable* Target);

	UPROPERTY(EditDefaultsOnly)
		float WalkSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly)
		float RunCoeff = 3.0f;
		
	UPROPERTY(EditDefaultsOnly)
		FRPGDice DamagePerFallDuration;

	UPROPERTY(EditDefaultsOnly)
		float SafeFallDuration = 1.0f;

	UFUNCTION(BlueprintCallable)
	void SetCoins(int Value);

	UPROPERTY(EditDefaultsOnly)
		int StarterCoins = 0;
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	void AddUnit(TSubclassOf<ARPGPlayerUnit> UnitClass);

	UPROPERTY(BlueprintReadWrite)
		int Coins = 0;

	const int UnitCapacity = 4;

	UPROPERTY(EditDefaultsOnly)
	int StarterUnits = 1;

	UFUNCTION()
		void OnUnitAvatarClicked(ARPGPlayerUnit* Unit, FName ButtonName);

	UFUNCTION()
		void OnCreatureDied(class ARPGCreature* Unit);

	int InteractablesInRange = 0;

	UFUNCTION()
		void OnInteractionColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnInteractionColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
		void OnAddUnitProposed(TSubclassOf<ARPGPlayerUnit> UnitClass);


	AActor* NearestInteractable = nullptr;

	bool Walking = false;
	bool Running = false;
	bool OnGround = false;

	FTimerHandle FallingHandle;

	UFUNCTION()
	void OnOnReachedJumpApex();	

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* PlayerCameraComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* InteractionCollider;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ARPGPlayerUnit> StarterUnitClass;

	class URPGRandomAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* SightSource;

	TArray<ARPGPlayerUnit*> Units = {};

	UPROPERTY()
	class ASceneCapture2D* MiniMapCamera = nullptr;

	AActor* GetNearestTarget(class UShapeComponent* Collider, bool ShouldBeVisible = true, bool ShouldBeInteractable = false);

	UFUNCTION()
	void OnUnitRecoveryStateChanged(AActor* Unit, bool IsInRecovery);

	ARPGPlayerUnit* FindFirstOutOfRecoveryUnit();

	void SetSelectedUnit(ARPGPlayerUnit* Unit);

	TWeakObjectPtr<ARPGPlayerUnit> SelectedUnit = nullptr;
};
