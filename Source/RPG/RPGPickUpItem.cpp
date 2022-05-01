// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPickUpItem.h"

/**
 * Sets default values
*/
ARPGPickUpItem::ARPGPickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void ARPGPickUpItem::OnInteracted()
{
	if (WasRecentlyRendered())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hi!"));
		Destroy();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Out of Screen!"));
	}
}
