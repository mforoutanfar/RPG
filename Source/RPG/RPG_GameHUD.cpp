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
#include "RPG_InteractablePing.h"
#include "RPGInteractable.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void URPG_GameHUD::OnStoryEventTriggered(FString EventName)
{
	if (StoryEventMap.Contains(EventName))
	{
		if (auto EventClass = StoryEventMap[EventName])
		{
			CurrentStoryEvent = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), EventClass);
			auto HorSlot = HorBox->AddChildToHorizontalBox(CurrentStoryEvent);
			HorSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			HorSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

			FSlateChildSize Size;

			HorSlot->SetSize(Size);

			Background->SetVisibility(ESlateVisibility::Visible);
			MiniMap->SetVisibility(ESlateVisibility::Collapsed);
		}
	}	
}

void URPG_GameHUD::CloseStoryEvent()
{
	if (CurrentStoryEvent)
	{
		CurrentStoryEvent->RemoveFromParent();
		CurrentStoryEvent = nullptr;

		Background->SetVisibility(ESlateVisibility::Collapsed);
		MiniMap->SetVisibility(ESlateVisibility::Visible);
	}
}

void URPG_GameHUD::ShowMessage(FString Message, float Duration)
{
	GetWorld()->GetTimerManager().ClearTimer(MessageTimerHandle);

	MessageText->SetText(FText::FromString(Message));
	
	GetWorld()->GetTimerManager().SetTimer(MessageTimerHandle, this, &URPG_GameHUD::HideMessage, Duration, false);

	MessageText->SetVisibility(ESlateVisibility::Visible);
}

void URPG_GameHUD::HideMessage()
{
	MessageText->SetVisibility(ESlateVisibility::Collapsed);
}

void URPG_GameHUD::NativeConstruct()
{
	Super::NativeConstruct();
	RPGEventManager->UnitAdded.AddDynamic(this, &URPG_GameHUD::OnUnitAdded);
	RPGEventManager->AttackOccured.AddDynamic(this, &URPG_GameHUD::OnAttackOccured);
	RPGEventManager->ItemWidgetPicked.AddDynamic(this, &URPG_GameHUD::OnItemWidgetPicked);	
	RPGEventManager->EquipmentItemReplaced.AddDynamic(this, &URPG_GameHUD::OnEquipmentItemReplaced);
	RPGEventManager->NearestInteractableChanged.AddDynamic(this, &URPG_GameHUD::OnNearestInteractableChanged);
	RPGEventManager->RemovePickedItemProposed.AddDynamic(this, &URPG_GameHUD::OnRemovePickedItemProposed);
	RPGEventManager->CoinChanged.AddDynamic(this, &URPG_GameHUD::OnCoinValueChanged);
}

void URPG_GameHUD::OnCoinValueChanged(int Value)
{		
	CoinValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), Value)));
}

void URPG_GameHUD::OnRemovePickedItemProposed()
{
	if (PickedItem)
	{
		PickedItem->RemoveFromParent();
		PickedItem = nullptr;
	}
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

void URPG_GameHUD::OnEquipmentItemReplaced(FRPGItemInfo PreviousItemInfo)
{
	PickedItem = CreateWidget<URPG_ItemWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ItemClass);
	PickedItem->Init(nullptr, PreviousItemInfo);

	Canvas->AddChildToCanvas(PickedItem);
	PickedItem->UpdateSizeForHUD();
	PickedItem->bShouldFollowMouse = true;
	PickedItem->FollowMouse(false);//So it won't jump on first frame.
}

void URPG_GameHUD::OnNearestInteractableChanged(AActor* NearestInteractable)
{
	InteractionPing->SetTarget(NearestInteractable);

	if (NearestInteractable)
	{
		auto Interactable = Cast<IRPGInteractable>(NearestInteractable);
		InteractionPing->SetPingString(Interactable->Execute_GetInteractableType(NearestInteractable));
		InteractionPing->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractionPing->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URPG_GameHUD::OnUnitAdded(ARPGPlayerUnit* Unit)
{
	auto Avatar = CreateWidget<URPG_AvatarWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), AvatarClass);
	Avatar->Init(Unit);
	auto VertBoxSlot = AvatarsBox->AddChildToVerticalBox(Avatar);
	
	VertBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

	VertBoxSlot->SetPadding(FMargin(10.0f, 0.0f, 10.0f, 0.0f));

	AvatarMap.Add(Unit, Avatar);
}

void URPG_GameHUD::OnAttackOccured(AActor* Attacker, FRPGAttackData Data, FRPGAttackResults Results)
{
	if (Attacker)
	{
		if (Cast<ARPGPlayerUnit>(Attacker) && !Data.Ranged)
		{
			auto Slash = CreateWidget<URPG_FollowerWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), SlashWidgetClass);
			Slash->SetTarget(Data.Target);
			Slash->CanvasSlot = Canvas->AddChildToCanvas(Slash);			 
			Slash->CanvasSlot->SetAutoSize(true);
			Slash->CanvasSlot->SetAlignment(FVector2D(0.5f,0.5f));
		}
	}

	if (Data.Target)
	{
		auto DamageNumber = CreateWidget<URPG_DamageNumberWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), DamageNumberWidgetClass);
		auto PlayerTarget = Cast<ARPGPlayerUnit>(Data.Target);

		if (!PlayerTarget)
		{
			DamageNumber->Init(Data.Target, Results);
		}
		else
		{
			DamageNumber->Init(AvatarMap[PlayerTarget], Results);
		}

		DamageNumber->CanvasSlot = Canvas->AddChildToCanvas(DamageNumber);
		DamageNumber->CanvasSlot->SetAutoSize(true);
		DamageNumber->CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	}

}

void URPG_GameHUD::OnOpenInventoryPressed(bool InventoryOpen)
{
	if (InventoryOpen)
	{
		InventoryGroup->SetVisibility(ESlateVisibility::Visible);
		Background->SetVisibility(ESlateVisibility::Visible);
		Equipment->SetVisibility(ESlateVisibility::Visible);
		MiniMap->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		InventoryGroup->SetVisibility(ESlateVisibility::Collapsed);
		Background->SetVisibility(ESlateVisibility::Collapsed);
		Equipment->SetVisibility(ESlateVisibility::Collapsed);
		MiniMap->SetVisibility(ESlateVisibility::Visible);
	}
}

void URPG_GameHUD::OnPausePressed(bool PauseMenuOpen)
{
	if (PauseMenuOpen)
	{
		PauseMenu->SetVisibility(ESlateVisibility::Visible);
		MiniMap->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		PauseMenu->SetVisibility(ESlateVisibility::Collapsed);
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
