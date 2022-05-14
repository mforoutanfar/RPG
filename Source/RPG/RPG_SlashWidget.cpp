// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_SlashWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "RPGPlayerUnit.h"

void URPG_SlashWidget::MakeProjectileAnimation(UWidget* InWidget, float initVelocity, float initAngle, float gravity, float life)
{
	Projectile = InWidget;

	auto AngleRad = FMath::DegreesToRadians(initAngle);
	Velocity = initVelocity * FVector2D(FMath::Cos(AngleRad), FMath::Sin(AngleRad));
	auto canSlot = Cast<UCanvasPanelSlot>(Projectile->Slot);
	Position = canSlot->GetPosition();
	Life = life;
	Gravity = gravity;

	ProjectileAnimationRunning = true;
}

void URPG_SlashWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ProjectileAnimationRunning)
	{
		ProjectileTimer += InDeltaTime;
		
		if (ProjectileTimer > Life)
		{
			RemoveFromParent();
		}

		auto NewVelocity = Velocity + FVector2D(0.0f, -Gravity) * InDeltaTime;
		auto NewPosition = Position + NewVelocity * InDeltaTime;

		Velocity = NewVelocity;
		Position = NewPosition;

		auto canSlot = Cast<UCanvasPanelSlot>(Projectile->Slot);
		canSlot->SetPosition(NewPosition);
	}
}

void URPG_SlashWidget::Init(AActor* InAttacker, AActor* InTarget, FRPGAttackResults InAttackResults)
{
	Target = InTarget;
	AttackResults = InAttackResults;
	Attacker = InAttacker;

	if (!Attacker )
	{
		Slash->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (!Cast<ARPGPlayerUnit>(Attacker))
	{
		Slash->SetVisibility(ESlateVisibility::Collapsed);
	}
}
