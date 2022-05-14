// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGAttackable.h"

#include "RPGCreature.generated.h"

UCLASS()
class RPG_API ARPGCreature : public ACharacter, public IRPGAttackable
{
	GENERATED_BODY()

public:

	enum class CreatureType : uint8
	{
		NONE,
		PLAYER,
		ENEMY,
		NPC
	};

	CreatureType CreatureType = CreatureType::NONE;

	// Sets default values for this character's properties
	ARPGCreature();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	virtual FRPGAttackResults OnAttacked(FRPGAttackData AttackData) override;

	bool IsInRecovery() { return InRecovery; };

	UFUNCTION(BlueprintCallable)
	void Attack();

	float MaxHP = 300.0f;
	float HP = 100.0f;

protected:
	bool InRecovery = false;
	void EnterRecovery(float Duration);
	void ExitRecovery();

	FName CreatureName = "Corpse";

	float MaxMana = 300.0f;
	float Mana = 300.0f;

	float MeleeDamage = 100.0f;

	float RangedDamage = 100.0f;

	class URPGRandomAudioComponent* AudioComponent;

	bool Dead = false;
	bool IsDead() { return Dead; }
	void Die();

	AActor* GetNearestAttackTarget(class UShapeComponent* Collider, bool ExcludeOwnType = true);

	FTimerHandle RecoveryTimerHandle;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* HitBox;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* MeleeSphere = nullptr;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* RangeSphere = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FVector ActionLocation;

	class URPGInventory* Inventory;

};
