// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameHUD.h"
#include "Components/Image.h"

#include "RPGPlayerUnit.h"
#include "RPGCreature.h"
#include "RPG_InventoryWidget.h"
#include "RPG_ItemWidget.h"
#include "RPGInventoryItem.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"
#include "RPG_AvatarWidget.h"
#include "RPG_DamageNumberWidget.h"
#include "RPG_MiniMapWidget.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

void URPG_GameHUD::NativeConstruct()
{
	Super::NativeConstruct();
	RPGEventManager->UnitAdded.AddDynamic(this, &URPG_GameHUD::OnUnitAdded);
	RPGEventManager->AttackOccured.AddDynamic(this, &URPG_GameHUD::OnAttackOccured);
	RPGEventManager->ItemWidgetPicked.AddDynamic(this, &URPG_GameHUD::OnItemWidgetPicked);	
	RPGEventManager->InventoryItemAdded.AddDynamic(this, &URPG_GameHUD::OnInventoryItemAdded);
}

void URPG_GameHUD::OnItemWidgetPicked(URPG_ItemWidget* ItemWidget)
{
	PickedItem = CreateWidget<URPG_ItemWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ItemClass);
	PickedItem->Init(nullptr, ItemWidget->ItemRef->ItemInformation);

	Canvas->AddChildToCanvas(PickedItem);
	PickedItem->UpdateSizeForHUD();
	PickedItem->bShouldFollowMouse = true;
	PickedItem->FollowMouse(false);//So it won't jump on first frame.

}

void URPG_GameHUD::OnInventoryItemAdded(URPGInventoryItem* Item, ARPGCreature* Creature)
{
	//Assuming the item was added via picked item in inventory. TODO: Better solution?
	if (PickedItem)
	{
		PickedItem->RemoveFromParent();
		PickedItem = nullptr;
	}
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
			Slash->CanvasSlot = Canvas->AddChildToCanvas(Slash);			 
			Slash->CanvasSlot->SetAutoSize(true);
			Slash->CanvasSlot->SetAlignment(FVector2D(0.5f,0.5f));
		}
	}

	if (Target)
	{
		if (!Cast<ARPGPlayerUnit>(Target))
		{
			auto DamageNumber = CreateWidget<URPG_DamageNumberWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), DamageNumberWidgetClass);
			DamageNumber->Init(Target, Results);
			DamageNumber->CanvasSlot = Canvas->AddChildToCanvas(DamageNumber);
			DamageNumber->CanvasSlot->SetAutoSize(true);
			DamageNumber->CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		}
	}

}

void URPG_GameHUD::OnOpenInventoryPressed(bool InventoryOpen)
{
	if (InventoryOpen)
	{
		Inventory->SetVisibility(ESlateVisibility::Visible);
		Background->SetVisibility(ESlateVisibility::Visible);
		Equipment->SetVisibility(ESlateVisibility::Visible);
		MiniMap->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Inventory->SetVisibility(ESlateVisibility::Collapsed);
		Background->SetVisibility(ESlateVisibility::Collapsed);
		Equipment->SetVisibility(ESlateVisibility::Collapsed);
		MiniMap->SetVisibility(ESlateVisibility::Visible);
	}
}

void URPG_GameHUD::RegisterOnMinimap(AActor* Actor, TEnumAsByte<MiniMap::ObjectType> Type)
{
	MiniMap->RegisterOnMinimap(Actor, Type);
}

void URPG_GameHUD::UnregisterFromMinimap(AActor* Actor)
{
	MiniMap->UnregisterFromMinimap(Actor);
}
