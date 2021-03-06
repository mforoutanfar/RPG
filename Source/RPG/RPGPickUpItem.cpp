// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPickUpItem.h"
#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"
#include "RPGRandomAudioComponent.h"
#include "RPG_MinimapComponent.h"

/**
 * Sets default values
*/
ARPGPickUpItem::ARPGPickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<USphereComponent>(FName("HitBox"));
	HitBox->SetCollisionProfileName(FName("Interactable"));

	ItemPicture = CreateDefaultSubobject<UBillboardComponent>(FName("ItemPicture"));
	ItemPicture->SetHiddenInGame(false);

	MinimapComponent = CreateDefaultSubobject<URPG_MinimapComponent>(FName("MinimapComponent"));
}

void ARPGPickUpItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ItemPicture->SetRelativeLocation(FVector::ZeroVector);
	ItemPicture->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ARPGPickUpItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MinimapComponent->UnregisterFromMiniMap();
	Super::EndPlay(EndPlayReason);
}

/**
 * Called when the game starts or when spawned
*/
void ARPGPickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
	MinimapComponent->RegisterOnMiniMap(MiniMap::LOOT);

	UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ItemInformation.GetSpritePath()));
	if (tmpTexture)
	{
		ItemPicture->SetSprite(tmpTexture);
	}

	SetRandomPrice();
}

void ARPGPickUpItem::SetRandomPrice()
{
	if (ItemInformation.ItemCategory == COIN)
	{
		ItemInformation.Price = FMath::RandRange(50, 100);
	}
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPGPickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGPickUpItem::OnInteracted_Implementation(bool Successful)
{	
	if (Successful)
	{
		Destroy();
	}
	else
	{
		HitBox->AddImpulse(7000*GetActorUpVector());
	}
}

bool ARPGPickUpItem::IsInteractable_Implementation()
{
	return true;
}

InteractableCat ARPGPickUpItem::GetInteractableType_Implementation()
{
	return ITEM;
}
