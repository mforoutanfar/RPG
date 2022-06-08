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
#include "RPG_Equipment.h"


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

	//For AI Sight Detection. TODO: Better alternative?
	Tags.Add(FName("Player"));
}


void ARPGPlayerUnit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ARPGPlayerUnit::OnHitboxBeginOverlap);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &ARPGPlayerUnit::OnHitboxEndOverlap);
}

/**
 * Called when the game starts or when spawned
*/
void ARPGPlayerUnit::BeginPlay()
{
	Super::BeginPlay();
	RPGEventManager->AddItemToInventoryProposed.AddDynamic(this, &ARPGPlayerUnit::OnAddItemToInventoryProposed);
	RPGEventManager->AddItemToEquipmentProposed.AddDynamic(this, &ARPGPlayerUnit::OnAddItemToEquipmentProposed);
		
	TSet<UPrimitiveComponent*> OverlappingComponents;
	HitBox->GetOverlappingComponents(OverlappingComponents);

	//TODO: OverlappingComponents is empty on BeginPlay for some reason.
	for (auto Comp : OverlappingComponents)
	{
		if (Comp->ComponentHasTag(FName("MeleeSphere")))
		{
			EnemiedInMeleeRange++;
		}
		else if (Comp->ComponentHasTag(FName("RangeSphere")))
		{
			EnemiedInRangedRange++;
		}
	}

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

void ARPGPlayerUnit::OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("MeleeSphere")))
	{
		EnemiedInMeleeRange++;
		UpdateSafetyState();
	}
	else if (OtherComp->ComponentHasTag(FName("RangeSphere")))
	{
		EnemiedInRangedRange++;
		UpdateSafetyState();
	}
}

void ARPGPlayerUnit::OnHitboxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("MeleeSphere")))
	{
		EnemiedInMeleeRange--;
		UpdateSafetyState();
	}
	else if (OtherComp->ComponentHasTag(FName("RangeSphere")))
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

void ARPGPlayerUnit::OnAddItemToEquipmentProposed(ARPGCreature* Creature, FRPGItemInfo ItemInfo)
{
	if (Creature == this)
	{
		Equipment->AddItem(ItemInfo);
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

