// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_ItemWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "RPGInventoryItem.h"
#include "Kismet/GameplayStatics.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"

float URPG_ItemWidget::InventoryScale = -1.0f;

void URPG_ItemWidget::Init(URPGInventoryItem* InItemRef, FRPGItemInfo InItemInfo)
{
	ItemRef = InItemRef;
	ItemInfo = InItemInfo;

	FString AssetName = "Item_" + ItemInfo.ItemName.ToString();
	FString PathToLoad = FString("/Game/Assets/Items/") + AssetName + FString(".") + AssetName;
	UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(PathToLoad)));
	ItemImage->SetBrushFromTexture(tmpTexture);
}

//TODO: Find a more elegant sizing way!
void URPG_ItemWidget::UpdateSizeForInventory()
{
	Cast<UCanvasPanelSlot>(Slot)->SetSize(FVector2D(100.0f * ItemInfo.Width, 100.0f * ItemInfo.Height));
	Cast<UCanvasPanelSlot>(Slot)->SetPosition(FVector2D(100.f * (ItemInfo.InventoryX), 100.f * (ItemInfo.InventoryY)));
}

void URPG_ItemWidget::UpdateSizeForHUD()
{
	Cast<UCanvasPanelSlot>(Slot)->SetSize(FVector2D(100.0f * ItemInfo.Width* InventoryScale, 100.0f * ItemInfo.Height* InventoryScale));
	Cast<UCanvasPanelSlot>(Slot)->SetPosition(FVector2D(100.f * (ItemInfo.InventoryX), 100.f * (ItemInfo.InventoryY)));
}

FReply URPG_ItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	RPGEventManager->ItemWidgetClicked.Broadcast(this);

	//Unhandled So that Inventorywidget can handle it.
	return FReply::Unhandled();
}

void URPG_ItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = false;

	RPGEventManager->InventoryItemRemoved.AddDynamic(this, &URPG_ItemWidget::OnItemRemoved);
}

void URPG_ItemWidget::OnItemRemoved(URPGInventoryItem* Item, ARPGCreature* Creature)
{
	if (Item == ItemRef)
	{
		RemoveFromParent();
	}
}

void URPG_ItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bShouldFollowMouse)
	{
		FollowMouse(true);
	}
}

void URPG_ItemWidget::FollowMouse(bool HasLag)
{
	auto MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	auto Dest = MousePos + FVector2D(30.0f, 30.0f);
	float Alpha = 0.3f;
	if (!HasLag)
	{
		Alpha = 1.0f;
	}
	Cast<UCanvasPanelSlot>(Slot)->SetPosition(FMath::Lerp(Cast<UCanvasPanelSlot>(Slot)->GetPosition(), Dest, Alpha));
}

