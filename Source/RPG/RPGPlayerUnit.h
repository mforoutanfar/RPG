// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGPlayerUnit.generated.h"

class IRPGAttackable;

DECLARE_DELEGATE_TwoParams(FRecoveryStateChanged, ARPGPlayerUnit*, bool);

/**
 *
*/
UCLASS()
class RPG_API ARPGPlayerUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * Sets default values for this actor's properties
	*/
	ARPGPlayerUnit();

	float MeleeDamage = 100.0f;
	float RangedDamage = 100.0f;

	void AttackTarget(IRPGAttackable* NearestMeleeTarget, IRPGAttackable* NearestRangedTarget);

	void InteractWithTarget(AActor* Target);

	int UnitIndex = -1;

protected:
	/**
	 * Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation 
	*/
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* HitBox;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FVector ActionLocation;

	bool InRecovery = false;
	void EnterRecovery(float Duration);
	void ExitRecovery();
		
	FTimerHandle RecoveryTimerHandle;

	class URPGRandomAudioComponent* AudioComponent;

	class URPGInventory* Inventory;

public:
	bool IsInRecovery() { return InRecovery; };

	FRecoveryStateChanged RecoveryStateChanged;
};
