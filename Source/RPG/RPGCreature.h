// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGAttackable.h"
#include "RPGInteractable.h"
#include "Perception/AIPerceptionTypes.h"

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

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditDefaultsOnly)
	class URPGBillboardVisuals* Visuals = nullptr;

	class URPGRandomAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* SightConfig;

	bool Dead = false;
	bool IsDead() { return Dead; }

	void Die();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FRPGAttackResults OnAttacked(FRPGAttackData AttackData) override;

	virtual InteractableCat GetInteractableType() override;
	virtual void OnInteracted(bool Successful) override;

	UPROPERTY(EditAnywhere);
	float HP = 100.0f;
};
