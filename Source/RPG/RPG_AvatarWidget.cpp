// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_AvatarWidget.h"
#include "RPGPlayerUnit.h"
#include "RPGPlayer.h"
#include "Components/Image.h"
#include "RPG_EventManager.h"

void URPG_AvatarWidget::Init(ARPGPlayerUnit* Unit)
{
	ReferencedUnit = Unit;
	URPG_EventManager::GetInstance()->RecoveryStateChanged.AddDynamic(this, &URPG_AvatarWidget::OnRecoveryStateChanged);
	URPG_EventManager::GetInstance()->UnitAttackedEnemy.AddDynamic(this, &URPG_AvatarWidget::OnUnitAttackedEnemy);
	URPG_EventManager::GetInstance()->SelectedUnitChanged.AddDynamic(this, &URPG_AvatarWidget::OnSelectedUnitChanged);
}

void URPG_AvatarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RecoveryIndicator->SetBrushTintColor(SafeColor);
	SelectedIndicator->SetBrushTintColor(DeselectedColor);
}

URPG_AvatarWidget::URPG_AvatarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FString Codes[7] = {"normal", "angry", "sad", "dead", "disappointed", "happy", "pain"};
		
	for (size_t i = 0; i < 7; i++)
	{
		FString AssetName = "avatar_dummy_" + Codes[i];
		FString PathToLoad = FString("/Game/Assets/Avatars/") + AssetName + FString(".") + AssetName;

		UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(PathToLoad)));
		AvatarMap.Add(AvatarState(i), tmpTexture);
	}
}

void URPG_AvatarWidget::OnRecoveryStateChanged(ARPGPlayerUnit* Unit, bool State)
{
	if (ReferencedUnit.Get() != Unit)
	{
		return;
	}

	if (State)
	{
		RecoveryIndicator->SetBrushTintColor(RecoveryColor);
	}
	else
	{
		//TODO: Change based on Unit Safety
		RecoveryIndicator->SetBrushTintColor(SafeColor);
	}
}

void URPG_AvatarWidget::OnUnitAttackedEnemy(ARPGPlayerUnit* Unit, FRPGAttackResults Results)
{
	if (ReferencedUnit.Get() != Unit)
	{
		return;
	}

	if (Results.TargetDied)
	{
		Portrait->SetBrushFromTexture(AvatarMap[HAPPY]);
	}
	else
	{
		Portrait->SetBrushFromTexture(AvatarMap[ANGRY]);
	}

	GetWorld()->GetTimerManager().ClearTimer(ResetAvatarHandle);
	GetWorld()->GetTimerManager().SetTimer(ResetAvatarHandle, this, &URPG_AvatarWidget::ResetAvatar, ResetDelay, true);
}

void URPG_AvatarWidget::OnSelectedUnitChanged(ARPGPlayerUnit* Unit)
{
	if (ReferencedUnit.Get() == Unit)
	{
		SelectedIndicator->SetBrushTintColor(SelectedColor);
	}
	else
	{
		SelectedIndicator->SetBrushTintColor(DeselectedColor);
	}
}

void URPG_AvatarWidget::ResetAvatar()
{
	Portrait->SetBrushFromTexture(AvatarMap[NORMAL]);
}