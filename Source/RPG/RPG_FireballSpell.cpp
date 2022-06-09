// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_FireballSpell.h"
#include "RPGCreature.h"
#include "Components/SphereComponent.h"
#include "RPG_Projectile.h"
#include "RPGPlayer.h"
#include "RPGRandomAudioComponent.h"

void URPG_FireballSpell::CastSpell()
{
	Super::CastSpell();

	if (ProjectileClass)
	{
		auto ModifActionLocation = Caster->GetActorRotation().RotateVector(Caster->ActionLocation);
		auto PointOfAction = Caster->GetActorLocation() + ModifActionLocation;
		auto Target = Caster->GetNearestAttackTarget(Caster->RangeSphere);

		FRotator Direction;

		if (Target)
		{
			Direction = (Target->GetActorLocation() - PointOfAction).Rotation();
		}
		else
		{
			if (GetWorld())
			{
				auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
				if (CameraManager)
				{
					Direction = CameraManager->GetCameraRotation();
				}
			}
			//Direction = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorForwardVector().Rotation();
		}

		auto Projectile = GetWorld()->SpawnActor<ARPG_Projectile>(ProjectileClass, PointOfAction, Direction);	
		Caster->AudioComponent->PlayRandom("fireball_cast");
	}
}
