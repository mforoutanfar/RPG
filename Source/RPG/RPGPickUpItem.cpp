// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPickUpItem.h"
#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"
#include "RPGRandomAudioComponent.h"

/**
 * Sets default values
*/
ARPGPickUpItem::ARPGPickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	HitBox->SetCollisionProfileName(FName("Interactable"));

	ItemPicture = CreateDefaultSubobject<UBillboardComponent>(FName("ItemPicture"));
	ItemPicture->SetHiddenInGame(false);
}

void ARPGPickUpItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ItemPicture->SetRelativeLocation(FVector::ZeroVector);
	ItemPicture->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ARPGPickUpItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterFromMiniMap(this);
	Super::EndPlay(EndPlayReason);
}

/**
 * Called when the game starts or when spawned
*/
void ARPGPickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterOnMiniMap(this, MiniMap::LOOT);

	UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ItemInformation.GetSpritePath()));
	if (tmpTexture)
	{
		ItemPicture->SetSprite(tmpTexture);
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

void ARPGPickUpItem::OnInteracted(bool Successful)
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

bool ARPGPickUpItem::IsInteractable()
{
	return true;
}

InteractableCat ARPGPickUpItem::GetInteractableType()
{
	return ITEM;
}
