// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_DamageNumberWidget.h"
#include "Components/CanvasPanelSlot.h"

void URPG_DamageNumberWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void URPG_DamageNumberWidget::MakeProjectileAnimation(UWidget* InWidget, float InitVelocity, float InitAngle, float gravity, float InLife)
{
	Projectile = InWidget;

	auto AngleRad = FMath::DegreesToRadians(InitAngle);
	Velocity = InitVelocity * FVector2D(FMath::Cos(AngleRad), FMath::Sin(AngleRad));
	auto CanSlot = Cast<UCanvasPanelSlot>(Projectile->Slot);
	Position = CanSlot->GetPosition();
	Life = InLife;
	Gravity = gravity;

	ProjectileAnimationRunning = true;
}

void URPG_DamageNumberWidget::Init(UObject* InTarget, FRPGAttackResults InAttackResults)
{
	SetTarget(InTarget);
	AttackResults = InAttackResults;
}
