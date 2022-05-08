// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGAttackData.generated.h"

/**
 * 
 */
USTRUCT()
struct FRPGAttackData
{
    GENERATED_BODY()
public:
    UPROPERTY()
        int PhysicalDamage = 0;

    UPROPERTY()
        TWeakObjectPtr<AActor> Attacker = nullptr;
};