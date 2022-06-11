// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Fireball.h"
#include "Components/SphereComponent.h"
#include "RPGAttackable.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARPG_Fireball::ARPG_Fireball()
{
	ExplosionSphereComponent = CreateDefaultSubobject<USphereComponent>("ExplosionSphereComponent");
	ExplosionSphereComponent->SetCollisionProfileName(FName("GeneralDamageSource"));
}


void ARPG_Fireball::OnConstruction(const FTransform& Transform)
{
	ExplosionSphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ARPG_Fireball::PlayExplosionAnimation_Implementation()
{
}

void ARPG_Fireball::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorld()->GetTimerManager().ClearTimer(LifeHandle);
	MovementComponent->StopMovementImmediately();
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TArray<AActor*> OverlappingActors;
	ExplosionSphereComponent->GetOverlappingActors(OverlappingActors);

	for (auto i : OverlappingActors)
	{
		if (auto Attackable = Cast<IRPGAttackable>(i))
		{
			if (Attackable->IsAttackable())
			{
				FRPGAttackData AttackData;
				FRPGAttackResults Results;

				AttackData.Attacker = this;
				AttackData.Target = i;
				AttackData.Accuracy = Accuracy;
				CalculateDamage(AttackData, Results);

				Attackable->OnAttacked(AttackData, Results);

				RPGEventManager->AttackOccured.Broadcast(this, AttackData, Results);
			}
		}
	}

	PlayExplosionAnimation();
}
