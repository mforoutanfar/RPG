// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameHUD.h"
#include "Components/Image.h"

#include "RPGPlayerUnit.h"
#include "RPGCreature.h"
#include "RPG_EventManager.h"
#include "RPG_AvatarWidget.h"
#include "RPG_FollowerWidget.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"

void URPG_GameHUD::NativeConstruct()
{
	URPG_EventManager::GetInstance()->UnitAdded.AddUObject(this, &URPG_GameHUD::AddAvatar);
	URPG_EventManager::GetInstance()->UnitAttackedEnemy.AddUObject(this, &URPG_GameHUD::OnUnitAttackedEnemy);
}

void URPG_GameHUD::AddAvatar(TWeakObjectPtr<ARPGPlayerUnit> Unit)
{
	auto Avatar = CreateWidget<URPG_AvatarWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), AvatarClass);
	Avatar->Init(Unit);
	AvatarsBox->AddChildToVerticalBox(Avatar);

	Cast<UVerticalBoxSlot>(Avatar->Slot)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
}

void URPG_GameHUD::OnUnitAttackedEnemy(TWeakObjectPtr<ARPGPlayerUnit> Unit, FRPGAttackResults Results)
{
	if (DamageWidgetClass)
	{
		auto Follower = CreateWidget<URPG_FollowerWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), DamageWidgetClass);
		Follower->Target = Cast<AActor>(Results.Target);
		Canvas->AddChildToCanvas(Follower);
	}
}

