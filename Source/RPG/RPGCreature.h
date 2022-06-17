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

	UFUNCTION()
	void OnRemoveItemProposed(ARPGCreature* Creature, URPGInventoryItem* Item);

	void OnAttacked_Implementation(FRPGAttackData& AttackData, FRPGAttackResults& Results) override;

	bool IsAttackable_Implementation() override;

	UFUNCTION()
	void OnConsumeItemProposed(FRPGItemInfo ItemInfo, ARPGCreature* Creature);

	bool IsInRecovery() { return InRecovery; };

	UFUNCTION(BlueprintCallable)
	virtual void BeginAttack();

	UFUNCTION(BlueprintCallable)
	void CastReadySpell();

	UPROPERTY(EditAnywhere)
	float MaxHP = 30000.0f;

	UPROPERTY(EditAnywhere)
	float HP = 30000.0f;

	float MaxMana = 300.0f;
	float Mana = 300.0f;

	UPROPERTY(EditAnywhere)
	float Accuracy = 1.0f;

	UPROPERTY(EditAnywhere)
	float BaseCriticalChance = 0.01f;

	UPROPERTY(EditAnywhere)
	float BaseCriticalMultiplier = 2.0f;

	UPROPERTY(EditAnywhere)
		float BaseArmor = 0.0f;

	UPROPERTY(EditAnywhere)
	FRPGDice BaseMeleeDamage;	

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ARPG_Projectile> ProjectileClass = nullptr;

	bool IsDead() { return Dead; }

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FVector ActionLocation;

	//Put in public so spells can access them. TODO: Better solution?
	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* MeleeSphere = nullptr;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* RangeSphere = nullptr;

	AActor* GetNearestAttackTarget(class UShapeComponent* Collider, bool ExcludeOwnType = true, bool ShouldBeVisible = true);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class URPGRandomAudioComponent* AudioComponent;	//

	class URPGInventory* Inventory;

	UPROPERTY(EditAnywhere)
		TArray<FRPGLootChance> StartingInventoryItems;		

	UPROPERTY(EditAnywhere)
		int Coins = 0;

protected:
	bool InRecovery = false;
	void EnterRecovery(float Duration);
	void ExitRecovery();

	UPROPERTY(EditDefaultsOnly)
	FName CreatureName = "Corpse";


	bool Dead = false;
	virtual void Die();

	FTimerHandle RecoveryTimerHandle;

	TWeakObjectPtr<UCapsuleComponent> HitBox;

	class URPG_Equipment* Equipment;

	FRPGAttackResults Attack();
	void CalculateMeleeDamage(FRPGAttackData &OutData, FRPGAttackResults &Results);

	//TODO: Probably for development only.
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class URPG_Spell>> SpellClassess = {};

	UPROPERTY()
	TArray<class URPG_Spell*> Spells = {};

	int ReadySpellIndex = 0;
};
