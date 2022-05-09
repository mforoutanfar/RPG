// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameHUD.h"
#include "Components/Image.h"

#include "RPGPlayerUnit.h"
#include "RPG_EventManager.h"
#include "RPG_AvatarWidget.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void URPG_GameHUD::NativeConstruct()
{
	URPG_EventManager::GetInstance()->UnitAdded.BindUObject(this, &URPG_GameHUD::AddAvatar);
}

void URPG_GameHUD::AddAvatar(ARPGPlayerUnit* Unit)
{
	auto Avatar = CreateWidget<URPG_AvatarWidget>(AvatarsBox, AvatarClass);
	Avatar->BindToPlayer(Unit);
	AvatarsBox->AddChildToVerticalBox(Avatar);

	Cast<UVerticalBoxSlot>(Avatar->Slot)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
}

