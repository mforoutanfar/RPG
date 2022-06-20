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
#include "RPG_HUD.h"
#include "RPG_GameHUD.h"

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
	RPGEventManager->ItemWidgetClicked.AddDynamic(this, &URPG_InventoryWidget::OnItemWidgetClicked);
	RPGEventManager->ContainerFocusStateChanged.AddDynamic(this, &URPG_InventoryWidget::OnContainerFocusStateChanged);
	RPGEventManager->ContainerClosed.AddDynamic(this, &URPG_InventoryWidget::OnContainerClosed);
}

void URPG_InventoryWidget::OnContainerClosed(AActor* Container)
{
	CanvasMap[Container]->RemoveFromParent();
	CanvasMap.Remove(Container);
}

void URPG_InventoryWidget::OnContainerFocusStateChanged(AActor* Container, bool IsFocused)
{
	if (IsFocused)
	{
		if (!CanvasMap.Contains(Container))
		{
			//Assuming we have max 4 units. TODO: Better solution?
			int UnitIndex = 5;

			auto Canvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), FName(FString("Canvas") + FString::Printf(TEXT("%d"), UnitIndex)));
			InventorySwitcher->AddChild(Canvas);
			TWeakObjectPtr<AActor> WeakPtr = Container;
			CanvasMap.Add(WeakPtr, Canvas);
		}

		InventorySwitcher->SetActiveWidget(CanvasMap[Container]);
	}
}

void URPG_InventoryWidget::OnItemWidgetClicked(URPG_ItemWidget* ItemWidget, FName ButtonName)
{
	if (IsItemWidgetInInventoryWidget(ItemWidget))//Make sure it's not in equipment widget. TODO: Looks too complicated. Solution?
	{
		if (ButtonName == "LeftMouseButton")
		{
			LeftClickedItemWidget = ItemWidget;
		}
		else if (ButtonName == "RightMouseButton")
		{
			RightClickedItemWidget = ItemWidget;
		}
	}
}

bool URPG_InventoryWidget::IsItemWidgetInInventoryWidget(URPG_ItemWidget* ItemWidget)
{
	auto Par = ItemWidget->GetParent();

	for (auto i : CanvasMap)
	{
		if (i.Value == Par)
		{
			return true;
		}
	}

	return false;
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

void URPG_InventoryWidget::OnInventoryItemAdded(URPGInventoryItem* Item, AActor* Owner)
{
	if (CanvasMap.Contains(Owner))
	{
		if (auto Canvas = CanvasMap[Owner])
		{
			auto ItemWidget = CreateWidget<URPG_ItemWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ItemClass);
			Canvas->AddChildToCanvas(ItemWidget);

			ItemWidget->Init(Item, Item->ItemInformation);
			ItemWidget->UpdateSizeForInventory();

			Item->RefWidget = ItemWidget;
		}
	}
}

FReply URPG_InventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto Button = InMouseEvent.GetEffectingButton();

	if (Button.GetFName() == "LeftMouseButton")
	{
		if (RPGGameHUD->PickedItem)
		{
			auto MousePos = InMouseEvent.GetScreenSpacePosition();

			auto BackgroundGeom = Background->GetCachedGeometry();
			auto BGParGeom = Background->GetParent()->GetCachedGeometry();
			auto BgPos = BGParGeom.LocalToAbsolute(BackgroundGeom.GetLocalPositionAtCoordinates(FVector2D(0.0f, 0.0f)));

			FVector2D RelPos = MousePos - BgPos;

			auto Size = BackgroundGeom.GetAbsoluteSize();
			auto RowHeight = Size.Y / Rows;
			auto ColWidth = Size.X / Cols;

			int Col = RelPos.X / ColWidth;
			int Row = RelPos.Y / RowHeight;

			auto CurrentCanvas = InventorySwitcher->GetActiveWidget();
			auto RefUnit = CanvasMap.FindKey(Cast<UCanvasPanel>(CurrentCanvas))->Get();

			RPGEventManager->AddItemToInventoryProposed.Broadcast(RefUnit, RPGGameHUD->PickedItem->ItemInfo, Row, Col);
		}
		else if (LeftClickedItemWidget)
		{
			RPGEventManager->ItemWidgetPicked.Broadcast(LeftClickedItemWidget);
		}
	}
	else if (Button.GetFName() == "RightMouseButton")
	{
		if (RPGGameHUD->PickedItem && RightClickedItemWidget)
		{
			auto PickedInfo = RPGGameHUD->PickedItem->ItemInfo;
			auto ClickedInfo = RightClickedItemWidget->ItemInfo;

			if (PickedInfo.ItemCategory == ItemCategory::INGREDIENTS && ClickedInfo.ItemCategory == ItemCategory::INGREDIENTS)
			{
				auto ResultingItemName = RPGGameState->GetResultingItem(PickedInfo.ItemName, ClickedInfo.ItemName);

				if (ResultingItemName != "")
				{
					auto CurrentCanvas = InventorySwitcher->GetActiveWidget();

					auto RefUnit = Cast<ARPGCreature>(CanvasMap.FindKey(Cast<UCanvasPanel>(CurrentCanvas))->Get());

					RPGEventManager->RemoveItemProposed.Broadcast(RefUnit, RightClickedItemWidget->ItemRef);
					
					FRPGItemInfo ResultingInfo;
					//TODO: Need a database or factory system for item info
					ResultingInfo.ItemName = ResultingItemName;
					ResultingInfo.HP = FMath::Max(PickedInfo.HP, ClickedInfo.HP);
					ResultingInfo.Mana = FMath::Max(PickedInfo.Mana, ClickedInfo.Mana);
					ResultingInfo.Width = FMath::Max(PickedInfo.Width, ClickedInfo.Width);
					ResultingInfo.Height = FMath::Max(PickedInfo.Height, ClickedInfo.Height);
					ResultingInfo.ItemCategory = ItemCategory::ItemCat::CONSUMABLE;

					RPGEventManager->AddItemToInventoryProposed.Broadcast(RefUnit, ResultingInfo, ClickedInfo.InventoryY, ClickedInfo.InventoryX);
				}
			}
		}
	}

	LeftClickedItemWidget = nullptr;
	RightClickedItemWidget = nullptr;

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