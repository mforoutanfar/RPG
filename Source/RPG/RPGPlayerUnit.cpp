// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerUnit.h"

#include "Components/BoxComponent.h"
#include "RPG\RPGInteractable.h"
#include "RPG\RPGAttackable.h"


/**
 * Sets default values
*/
ARPGPlayerUnit::ARPGPlayerUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(RootComponent);
	HitBox->SetCollisionProfileName(FName("PlayerHitBox"));
	HitBox->ComponentTags.Add(FName("HitBox"));

	ActionLocation = FVector(0.0f,0.0f,30.0f);
}

void ARPGPlayerUnit::AttackTarget(IRPGAttackable* NearestMeleeTarget, IRPGAttackable* NearestRangedTarget)
{
	FRPGAttackData attackData;
	attackData.PhysicalDamage = 30.0f;
	attackData.Attacker = this;

	if (NearestMeleeTarget)
	{
		if (MeleeDamage > 0)
		{
			NearestMeleeTarget->OnAttacked(attackData);
		}
		
		EnterRecovery(10.0f);
	}
	else if (NearestRangedTarget)
	{
		if (RangedDamage > 0)
		{
			NearestRangedTarget->OnAttacked(attackData);
		}

		EnterRecovery(10.0f);
	}
}

void ARPGPlayerUnit::InteractWithTarget(AActor* Target)
{
	Cast<IRPGInteractable>(Target)->OnInteracted();
}

void ARPGPlayerUnit::OnConstruction(const FTransform& Transform)
{
}

/**
 * Called when the game starts or when spawned
*/
void ARPGPlayerUnit::BeginPlay()
{
	Super::BeginPlay();	
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPGPlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGPlayerUnit::EnterRecovery(float Duration)
{
	InRecovery = true;
	GetWorldTimerManager().SetTimer(RecoveryTimerHandle, this, &ARPGPlayerUnit::ExitRecovery, Duration, false);
	RecoveryStateChanged.ExecuteIfBound(this, true);
}

void ARPGPlayerUnit::ExitRecovery()
{
	InRecovery = false;
	GetWorldTimerManager().ClearTimer(RecoveryTimerHandle);
	RecoveryStateChanged.ExecuteIfBound(this, false);
}

