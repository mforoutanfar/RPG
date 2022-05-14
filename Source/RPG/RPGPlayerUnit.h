// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFunctionLibrary.h"
#include "RPGAttackable.h"

#include "RPGPlayerUnit.generated.h"

/**
 *
*/
UCLASS()
class RPG_API ARPGPlayerUnit : public AActor, public IRPGAttackable
{
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	*/
	ARPGPlayerUnit();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	void AttackTarget(IRPGAttackable* NearestMeleeTarget, IRPGAttackable* NearestRangedTarget);

	void InteractWithTarget(AActor* Target);

	int UnitIndex = -1;

	bool IsInRecovery() { return InRecovery; };

	float MaxHP = 300.0f;

	float MaxMana = 300.0f;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

	float MeleeDamage = 100.0f;

	float RangedDamage = 100.0f;

	virtual FRPGAttackResults OnAttacked(FRPGAttackData AttackData) override;

	float HP = 300.0f;

	float Mana = 300.0f;

	bool Dead = false;
	bool IsDead() { return Dead; }
	void Die();

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
};
