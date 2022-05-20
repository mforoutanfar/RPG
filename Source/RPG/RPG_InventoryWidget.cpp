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
#include "Blueprint/WidgetLayoutLibrary.h"

int32 URPG_InventoryWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 ret = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	auto a = Background->GetCachedGeometry().GetAbsoluteSize().Y / UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	auto b = Background->GetCachedGeometry().GetLocalSize().Y;
	URPG_ItemWidget::InventoryScale = a / b;

	return ret;
}

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

FReply URPG_InventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto MousePos = InMouseEvent.GetScreenSpacePosition();

	auto BackgroundGeom = Background->GetCachedGeometry();
	auto BGParGeom = Background->GetParent()->GetCachedGeometry();
	auto BgPos = BGParGeom.LocalToAbsolute(BackgroundGeom.GetLocalPositionAtCoordinates(FVector2D(0.0f, 0.0f)));

	FVector2D RelPos = MousePos - BgPos;

	auto Size = BackgroundGeom.GetAbsoluteSize();
	auto RowHeight = Size.Y / Rows;
	auto ColWidth = Size.X / Cols;

	int X = RelPos.X / ColWidth;
	int Y = RelPos.Y / RowHeight;

	return FReply::Handled();
}

//void URPG_InventoryWidget::UpdateInventoryScale()
//{
//	//TODO: Hack! Find a better way.
//	FTimerHandle TimerHandle;
//	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URPG_InventoryWidget::DoUpdateInventoryScale, 0.1f, false);
//}
//void URPG_InventoryWidget::DoUpdateInventoryScale()
//{
//	auto a = Background->GetCachedGeometry().GetAbsoluteSize().Y / UWidgetLayoutLibrary::GetViewportScale(this);
//	auto b = Background->GetCachedGeometry().GetLocalSize().Y;
//	URPG_ItemWidget::InventoryScale = a / b;
//}