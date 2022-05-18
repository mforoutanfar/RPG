// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_InventoryWidget.h"
#include "RPGPlayerUnit.h"
#include "RPG_ItemWidget.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"
#include "RPGInventoryItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"

void URPG_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RPGEventManager->InventoryItemAdded.AddDynamic(this, &URPG_InventoryWidget::OnInventoryItemAdded);
	RPGEventManager->UnitAdded.AddDynamic(this, &URPG_InventoryWidget::OnUnitAdded);
	RPGEventManager->SelectedUnitChanged.AddDynamic(this, &URPG_InventoryWidget::OnSelectedUnitChanged);
}

void URPG_InventoryWidget::OnUnitAdded(ARPGPlayerUnit* Unit)
{
	int UnitIndex = Unit->UnitIndex;
	auto Canvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), FName(FString("Canvas") + FString::Printf(TEXT("%d"), UnitIndex)));
	InventorySwitcher->AddChild(Canvas);
	TWeakObjectPtr<ARPGPlayerUnit> WeakPtr = Unit;
	CanvasMap.Add(WeakPtr, Canvas);
}

void URPG_InventoryWidget::OnSelectedUnitChanged(ARPGPlayerUnit* Unit)
{
	if (Unit)
	{
		InventorySwitcher->SetActiveWidget(CanvasMap[Unit]);
	}
}

void URPG_InventoryWidget::OnInventoryItemAdded(URPGInventoryItem* Item, ARPGCreature* Creature)
{
	if (auto PlayerUnit = Cast<ARPGPlayerUnit>(Creature))
	{
		if (auto Canvas = CanvasMap[PlayerUnit])
		{
			auto ItemWidget = CreateWidget<URPG_ItemWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ItemClass);
			Canvas->AddChildToCanvas(ItemWidget);

			ItemWidget->Init(Item);
		}
	}
}

//TODO: Hack! Find a better way.
void URPG_InventoryWidget::UpdateInventoryScale()
{
	if (URPG_ItemWidget::InventoryScale < 0)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URPG_InventoryWidget::DoUpdateInventoryScale, 0.1f, false);
	}
}
void URPG_InventoryWidget::DoUpdateInventoryScale()
{
	auto a = Background->GetCachedGeometry().GetAbsoluteSize().Y / UWidgetLayoutLibrary::GetViewportScale(this);
	auto b = Background->GetCachedGeometry().GetLocalSize().Y;
	URPG_ItemWidget::InventoryScale = a / b;
}