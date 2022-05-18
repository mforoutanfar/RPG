// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCreature.h"
#include "Perception/AIPerceptionTypes.h"
#include "RPGInteractable.h"
#include "RPG_Minimappable.h"

#include "RPGUnit.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGUnit : public ARPGCreature, public IRPGInteractable, public IRPG_Minimappable
{
	GENERATED_BODY()

public:
	ARPGUnit();

	virtual void BeginPlay() override;
	
	virtual InteractableCat GetInteractableType() override;
	virtual void OnInteracted(bool Successful) override;

	UFUNCTION(BlueprintCallable)
		void SetIsWalking(bool IsWalking);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class URPGBillboardVisuals* Visuals = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UAIPerceptionComponent* PerceptionComponent = nullptr;

	int SpeedZeroCounter = 0;

	bool Walking = false;

	virtual void Die() override;

protected:

};
