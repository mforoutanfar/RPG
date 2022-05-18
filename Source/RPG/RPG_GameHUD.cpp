// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameHUD.h"
#include "Components/Image.h"

#include "RPGPlayerUnit.h"
#include "RPGCreature.h"
#include "RPG_InventoryWidget.h"
#include "RPG_ItemWidget.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"
#include "RPG_AvatarWidget.h"
#include "RPG_DamageNumberWidget.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void URPG_GameHUD::NativeConstruct()
{
	Super::NativeConstruct();
	RPGEventManager->UnitAdded.AddDynamic(this, &URPG_GameHUD::OnUnitAdded);
	RPGEventManager->AttackOccured.AddDynamic(this, &URPG_GameHUD::OnAttackOccured);
	RPGEventManager->ItemWidgetPicked.AddDynamic(this, &URPG_GameHUD::OnItemWidgetPicked);
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
	if (Attacker)
	{
		if (Cast<ARPGPlayerUnit>(Attacker) && !Results.Ranged)
		{
			auto Slash = CreateWidget<URPG_FollowerWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), SlashWidgetClass);
			Slash->Target = Target;
			Canvas->AddChildToCanvas(Slash);
		}
	}

	if (Target)
	{
		if (!Cast<ARPGPlayerUnit>(Target))
		{
			auto DamageNumber = CreateWidget<URPG_DamageNumberWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), DamageNumberWidgetClass);
			DamageNumber->Init(Target, Results);
			Canvas->AddChildToCanvas(DamageNumber);
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
