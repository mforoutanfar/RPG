// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGAttackable.h"
#include "RPGInteractable.h"

#include "RPGCreature.generated.h"

UCLASS()
class RPG_API ARPGCreature : public ACharacter, public IRPGAttackable, public IRPGInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCreature();

	FName CreatureName = "Corpse";

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class URPGBillboardVisuals* Visuals = nullptr;

	class URPGRandomAudioComponent* AudioComponent;

	bool Dead = false;
	bool IsDead() { return Dead; }

	void Die();

	UFUNCTION()
		virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnAttacked(FRPGAttackData AttackData) override;

	virtual void OnInteracted() override;

	float HP = 100.0f;
};
