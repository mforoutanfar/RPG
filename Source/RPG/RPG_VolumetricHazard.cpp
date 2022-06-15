// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_VolumetricHazard.h"
#include "Components/StaticMeshComponent.h"
#include "RPGAttackable.h"
#include "RPG_GameStateBase.h"
#include "RPG_EventManager.h"

/**
 * Sets default values
*/
ARPG_VolumetricHazard::ARPG_VolumetricHazard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collider"));
	Collider->SetCollisionProfileName(FName("GeneralDamageSource"));
}

/**
 * Called when the game starts or when spawned
*/
void ARPG_VolumetricHazard::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(DamageTimer, this, &ARPG_VolumetricHazard::ApplyDamage, DamageInterval, true);
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPG_VolumetricHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARPG_VolumetricHazard::ApplyDamage()
{
	Collider->GetOverlappingActors(OverlappingActors, AActor::StaticClass());
	for (auto Object : OverlappingActors)
	{
		if (Object->Implements<URPGAttackable>())
		{			
			auto Attackable = Cast<IRPGAttackable>(Object);
			if (Attackable ->Execute_IsAttackable(Object))
			{
				FRPGAttackData AttackData;
				FRPGAttackResults Results;

				AttackData.Attacker = this;
				AttackData.Target = Object;
				AttackData.Accuracy = 1.0f;

				auto Damage = DamagePerSecond.GetResult() * DamageInterval;

				AttackData.Damage = Damage;

				Attackable->Execute_OnAttacked(Object, AttackData, Results);

				RPGEventManager->AttackOccured.Broadcast(this, AttackData, Results);
			}
		}
	}
}
