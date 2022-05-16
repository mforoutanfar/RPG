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
	Super::OnConstruction(Transform);
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
