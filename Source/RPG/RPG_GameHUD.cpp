// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameHUD.h"
#include "Components/Image.h"

#include "RPGPlayerUnit.h"
#include "RPGCreature.h"
#include "RPG_InventoryWidget.h"
#include "RPG_ItemWidget.h"
#include "RPG_EventManager.h"
#include "RPG_AvatarWidget.h"
#include "RPG_SlashWidget.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void URPG_GameHUD::NativeConstruct()
{
	Super::NativeConstruct();
	URPG_EventManager::GetInstance()->UnitAdded.AddDynamic(this, &URPG_GameHUD::OnUnitAdded);
	URPG_EventManager::GetInstance()->AttackOccured.AddDynamic(this, &URPG_GameHUD::OnAttackOccured);
	URPG_EventManager::GetInstance()->ItemWidgetPicked.AddDynamic(this, &URPG_GameHUD::OnItemWidgetPicked);
}

void URPG_GameHUD::OnItemWidgetPicked(URPG_ItemWidget* ItemWidget)
{
	Canvas->AddChildToCanvas(ItemWidget);
	ItemWidget->UpdateSizeForHUD();	
	ItemWidget->FollowMouse();//So it won't jump on first frame.
}

void URPG_GameHUD::OnUnitAdded(ARPGPlayerUnit* Unit)
{
	auto Avatar = CreateWidget<URPG_AvatarWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), AvatarClass);
	Avatar->Init(Unit);
	AvatarsBox->AddChildToVerticalBox(Avatar);

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

void URPG_GameHUD::OnOpenInventoryPressed(bool InventoryOpen)
{
	if (InventoryOpen)
	{
		Inventory->SetVisibility(ESlateVisibility::Visible);
		Blur->SetVisibility(ESlateVisibility::Visible);

		//TODO: Hack! Find a better way.
		Cast<URPG_InventoryWidget>(Inventory)->UpdateInventoryScale();
		//
	}
	else
	{
		Inventory->SetVisibility(ESlateVisibility::Collapsed);
		Blur->SetVisibility(ESlateVisibility::Collapsed);
	}
}
