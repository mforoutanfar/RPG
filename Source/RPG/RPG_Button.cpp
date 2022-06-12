// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Button.h"
#include "Components/SphereComponent.h"

/**
 * Sets default values
*/
ARPG_Button::ARPG_Button()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	HitBox->SetCollisionProfileName(FName("Interactable"));
}

void ARPG_Button::OnInteracted(bool Successful)
{
	if (!IsPressed)
	{
		RPGButtonPressed.Broadcast(this);
		SetIsPressed(true);
	}
}

void ARPG_Button::SetIsPressed(bool InIsPressed)
{
	IsPressed = InIsPressed;
}

bool ARPG_Button::IsInteractable()
{
	return !IsPressed;
}

InteractableCat ARPG_Button::GetInteractableType()
{
	return InteractableCat::BUTTON;
}

/**
 * Called when the game starts or when spawned
*/
void ARPG_Button::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPG_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPG_Button::OnRPGButtonFunctionFinished()
{
	IsPressed = false;
}

