// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPG_Spell.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_API URPG_Spell : public UObject
{
	GENERATED_BODY()
	
public:

	class ARPGCreature* Caster = nullptr;

	virtual void CastSpell();

	UPROPERTY(EditDefaultsOnly)
	float RequiredMana = 10.0f;

	UPROPERTY(EditDefaultsOnly)
		float RecoveryDuration = 2.0f;

};
