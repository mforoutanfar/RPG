// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerUnit.h"

#include "Components/BoxComponent.h"
#include "RPGInteractable.h"
#include "RPGAttackable.h"

#include "GameFramework\CharacterMovementComponent.h"
#include "RPGRandomAudioComponent.h"
#include "RPGInventory.h"
#include "RPGPickupItem.h"
#include "RPG_EventManager.h"

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

	AudioComponent = CreateDefaultSubobject<URPGRandomAudioComponent>(FName("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	Inventory = CreateDefaultSubobject<URPGInventory>(FName("Inventory"));

	ActionLocation = FVector(0.0f, 0.0f, 30.0f);
}

void ARPGPlayerUnit::AttackTarget(IRPGAttackable* NearestMeleeTarget, IRPGAttackable* NearestRangedTarget)
{
	FRPGAttackData attackData;
	attackData.PhysicalDamage = 30.0f;
	attackData.Attacker = this;
	FRPGAttackResults Results;

	if (NearestMeleeTarget)
	{
		if (MeleeDamage > 0)
		{
			Results = NearestMeleeTarget->OnAttacked(attackData);
		}

		EnterRecovery(2.0f);
	}
	else if (NearestRangedTarget)
	{
		if (RangedDamage > 0)
		{
			Results = NearestRangedTarget->OnAttacked(attackData);
		}

		EnterRecovery(2.0f);
	}

	URPG_EventManager::GetInstance()->UnitAttackedEnemy.Broadcast(this, Results);
}

void ARPGPlayerUnit::InteractWithTarget(AActor* Target)
{
	auto Interactable = Cast<IRPGInteractable>(Target);
	auto Type = Interactable->GetInteractableType();

	if (Type == ITEM)
	{
		auto Item = Cast<ARPGPickUpItem>(Target);

		bool Successful = Inventory->AddItem(Item->ItemInformation);
		Interactable->OnInteracted(Successful);

		//TODO: Put here because pickupItem is destroyed along with its audio component! 
		if (Successful)
		{
			AudioComponent->PlayRandom("cash");
		}
	}
}

void ARPGPlayerUnit::OnConstruction(const FTransform& Transform)
{
}

FRPGAttackResults ARPGPlayerUnit::OnAttacked(FRPGAttackData AttackData)
{
	FRPGAttackResults Results;
	Results.Target = this;

	float DamageDealt = 0.0f;
	DamageDealt += AttackData.PhysicalDamage;
	HP -= DamageDealt;

	Results.DamageDealt = DamageDealt;

	AudioComponent->PlayRandom("hit");

	if (HP <= 0)
	{
		Die();
		Results.TargetDied = true;
	}
	else
	{
		EnterRecovery(1.0f);
	}

	return Results;
}

void ARPGPlayerUnit::Die()
{
	Dead = true;
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
	URPG_EventManager::GetInstance()->RecoveryStateChanged.Broadcast(this, true);
}

void ARPGPlayerUnit::ExitRecovery()
{
	InRecovery = false;
	GetWorldTimerManager().ClearTimer(RecoveryTimerHandle);
	URPG_EventManager::GetInstance()->RecoveryStateChanged.Broadcast(this, false);
}