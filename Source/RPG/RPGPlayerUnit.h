// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFunctionLibrary.h"
#include "RPGAttackable.h"
#include "RPGCreature.h"

#include "RPGPlayerUnit.generated.h"

/**
 *
*/
UCLASS()
class RPG_API ARPGPlayerUnit : public ARPGCreature
{
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	*/
	ARPGPlayerUnit();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	void InteractWithTarget(AActor* Target);

	int UnitIndex = -1;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

private:

};
