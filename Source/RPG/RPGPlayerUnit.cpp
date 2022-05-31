// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerUnit.h"

#include "Components/BoxComponent.h"
#include "RPGInteractable.h"
#include "RPGAttackable.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework\CharacterMovementComponent.h"
#include "RPGRandomAudioComponent.h"
#include "RPGPickupItem.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"
#include "RPGInventory.h"


/**
 * Sets default values
*/
ARPGPlayerUnit::ARPGPlayerUnit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreatureType = CreatureType::PLAYER;

	ActionLocation = FVector(0.0f, 0.0f, 30.0f);

	GetMovementComponent()->DestroyComponent();
	GetMovementComponent()->SetActive(false);

	MeleeSphere->SetCollisionProfileName(FName("PlayerDamageSource"));
	RangeSphere->SetCollisionProfileName(FName("PlayerDamageSource"));
	HitBox->SetCollisionProfileName(FName("PlayerHitBox"));
}


void ARPGPlayerUnit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	MeleeSphere->OnComponentBeginOverlap.AddDynamic(this, &ARPGPlayerUnit::OnMeleeSphereBeginOverlap);
	MeleeSphere->OnComponentEndOverlap.AddDynamic(this, &ARPGPlayerUnit::OnMeleeSphereEndOverlap);
	RangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ARPGPlayerUnit::OnRangeSphereBeginOverlap);
	RangeSphere->OnComponentEndOverlap.AddDynamic(this, &ARPGPlayerUnit::OnRangeSphereEndOverlap);
}

void ARPGPlayerUnit::OnMeleeSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionProfileName() == FName("EnemyHitBox"))
	{
		EnemiedInMeleeRange++;
		UpdateSafetyState();
	}
}

void ARPGPlayerUnit::OnMeleeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetCollisionProfileName() == FName("EnemyHitBox"))
	{
		EnemiedInMeleeRange--;
		UpdateSafetyState();
	}
}

void ARPGPlayerUnit::OnRangeSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionProfileName() == FName("EnemyHitBox"))
	{
		EnemiedInRangedRange++;
		UpdateSafetyState();
	}
}

void ARPGPlayerUnit::OnRangeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetCollisionProfileName() == FName("EnemyHitBox"))
	{
		EnemiedInRangedRange--;
		UpdateSafetyState();
	}
}

void ARPGPlayerUnit::OnAddItemToInventoryProposed(ARPGCreature* Creature, FRPGItemInfo ItemInfo, int ProposedRow, int ProposedCol)
{
	if (Creature == this)
	{
		Inventory->AddItem(ItemInfo, ProposedRow, ProposedCol);
	}
}

void ARPGPlayerUnit::UpdateSafetyState()
{
	if (EnemiedInMeleeRange > 0)
	{
		CurrentSafetyState = UnitSafety::SafetyState::DANGER;
	}
	else if (EnemiedInRangedRange > 0)
	{
		CurrentSafetyState = UnitSafety::SafetyState::WARNING;
	}
	else
	{
		CurrentSafetyState = UnitSafety::SafetyState::SAFE;
	}

	RPGEventManager->SafetyStateChanged.Broadcast(this, CurrentSafetyState);
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

/**
 * Called when the game starts or when spawned
*/
void ARPGPlayerUnit::BeginPlay()
{
	Super::BeginPlay();
	RPGEventManager->AddItemToInventoryProposed.AddDynamic(this, &ARPGPlayerUnit::OnAddItemToInventoryProposed);
	UpdateSafetyState();
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPGPlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
