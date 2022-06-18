// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCreature.h"
#include "Perception/AIPerceptionTypes.h"
#include "RPGInteractable.h"

#include "RPGUnit.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGUnit : public ARPGCreature, public IRPGInteractable
{
	GENERATED_BODY()

public:
	void RPGSetActive(bool Active);
	ARPGUnit();

	virtual void BeginPlay() override;

	InteractableCat GetInteractableType_Implementation() override;
	void OnInteracted_Implementation(bool Successful) override;

	bool IsInteractable_Implementation() override;

	UFUNCTION(BlueprintCallable)
		void SetIsWalking(bool IsWalking);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class URPGBillboardVisuals* Visuals = nullptr;

	class URPG_MinimapComponent* MinimapComponent = nullptr;

	int SpeedZeroCounter = 0;

	bool Walking = false;

	virtual void Die() override;

	virtual void BeginAttack() override;

protected:

	UFUNCTION(BlueprintNativeEvent)
	void ShowSpawnEffect();

};
