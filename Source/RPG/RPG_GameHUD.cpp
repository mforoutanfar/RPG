// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameHUD.h"
#include "Components/Image.h"

#include "RPGPlayerUnit.h"
#include "RPGCreature.h"
#include "RPG_EventManager.h"
#include "RPG_AvatarWidget.h"
#include "RPG_SlashWidget.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"

void URPG_GameHUD::NativeConstruct()
{
	URPG_EventManager::GetInstance()->UnitAdded.AddDynamic(this, &URPG_GameHUD::OnUnitAdded);
	URPG_EventManager::GetInstance()->AttackOccured.AddDynamic(this, &URPG_GameHUD::OnAttackOccured);
}

void URPG_GameHUD::OnUnitAdded(ARPGPlayerUnit* Unit)
{
	auto Avatar = CreateWidget<URPG_AvatarWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), AvatarClass);
	Avatar->Init(Unit);
	AvatarsBox->AddChildToVerticalBox(Avatar);

	//Cast<UVerticalBoxSlot>(Avatar->Slot)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
	Cast<UVerticalBoxSlot>(Avatar->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
}

void URPG_GameHUD::OnAttackOccured(AActor* Attacker, AActor* Target, FRPGAttackResults Results)
{
	if (auto CreatTarget = Cast<ARPGCreature>(Target))
	{
		if (CreatTarget->CreatureType == ARPGCreature::CreatureType::ENEMY)
		{
			if (DamageWidgetClass)
			{
				auto Slash = CreateWidget<URPG_SlashWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), DamageWidgetClass);
				Slash->Init(Attacker, Results.Target.Get(), Results);
				Canvas->AddChildToCanvas(Slash);
			}
		}
	}
}

