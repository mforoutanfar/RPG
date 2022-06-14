// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Door.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"

#include "Components/BoxComponent.h"

/**
 * Sets default values
*/
ARPG_Door::ARPG_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetCollisionProfileName(FName("Interactable"));

}

/**
 * Called when the game starts or when spawned
*/
void ARPG_Door::BeginPlay()
{
	Super::BeginPlay();
	
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPG_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPG_Door::OnInteracted(bool Successful)
{
	if (Successful)
	{
		if (EventName != "")
		{
			RPGEventManager->StoryEventTriggered.Broadcast(EventName);
		}
	}
}

bool ARPG_Door::IsInteractable()
{
	return true;
}

InteractableCat ARPG_Door::GetInteractableType()
{
	return DOOR;
}
