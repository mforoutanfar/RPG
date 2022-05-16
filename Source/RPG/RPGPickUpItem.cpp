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

	ItemInformation.ItemName = "Sword";
	ItemInformation.Width = 1;
	ItemInformation.Height = 3;

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	HitBox->SetCollisionProfileName(FName("Interactable"));

	ItemPicture = CreateDefaultSubobject<UBillboardComponent>(FName("ItemPicture"));
	ItemPicture->SetHiddenInGame(false);

	FString AssetName = "Item_" + ItemInformation.ItemName.ToString();
	FString PathToLoad = FString("/Game/Assets/Items/") + AssetName + FString(".") + AssetName;

	UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(PathToLoad)));
	if (tmpTexture)
	{
		ItemPicture->SetSprite(tmpTexture);
	}

	AudioComponent = CreateDefaultSubobject<URPGRandomAudioComponent>(FName("AudioComponent"));
}

void ARPGPickUpItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ItemPicture->SetRelativeLocation(FVector::ZeroVector);
	ItemPicture->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	AudioComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

/**
 * Called when the game starts or when spawned
*/
void ARPGPickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
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
		AudioComponent->PlayRandom("error");
		HitBox->AddImpulse(7000*GetActorUpVector());
	}
}

InteractableCat ARPGPickUpItem::GetInteractableType()
{
	return ITEM;
}
