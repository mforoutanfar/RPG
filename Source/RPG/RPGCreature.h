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

	UFUNCTION()
	void OnItemWidgetPicked(class URPG_ItemWidget* Item);

	virtual FRPGAttackResults OnAttacked(FRPGAttackData AttackData) override;

	bool IsInRecovery() { return InRecovery; };

	UFUNCTION(BlueprintCallable)
	virtual void BeginAttack();

	UPROPERTY(EditAnywhere)
	float MaxHP = 30000.0f;

	UPROPERTY(EditAnywhere)
	float HP = 30000.0f;

	float MaxMana = 300.0f;
	float Mana = 300.0f;

	UPROPERTY(EditAnywhere)
	FRPGDice BaseMeleeDamage;	

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ARPG_Projectile> ProjectileClass = nullptr;

	bool IsDead() { return Dead; }

protected:
	bool InRecovery = false;
	void EnterRecovery(float Duration);
	void ExitRecovery();

	FName CreatureName = "Corpse";


	class URPGRandomAudioComponent* AudioComponent;

	bool Dead = false;
	virtual void Die();

	AActor* GetNearestAttackTarget(class UShapeComponent* Collider, bool ExcludeOwnType = true, bool ShouldBeVisible = true);

	FTimerHandle RecoveryTimerHandle;

	TWeakObjectPtr<UCapsuleComponent> HitBox;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* MeleeSphere = nullptr;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* RangeSphere = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FVector ActionLocation;

	class URPGInventory* Inventory;
	class URPG_Equipment* Equipment;

	FRPGAttackResults Attack();
	int CalculateMeleeDamage();
};
