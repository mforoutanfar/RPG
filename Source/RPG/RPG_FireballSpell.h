// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Spell.h"
#include "RPG_FireballSpell.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_FireballSpell : public URPG_Spell
{
	GENERATED_BODY()
	
	virtual void CastSpell() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ARPG_Projectile> ProjectileClass = nullptr;
};
