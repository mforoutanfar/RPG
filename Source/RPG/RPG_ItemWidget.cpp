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

void URPG_ItemWidget::Init(URPGInventoryItem* InItemRef)
{
	ItemRef = InItemRef;

	FString AssetName = "Item_" + InItemRef->ItemInformation.ItemName.ToString();
	FString PathToLoad = FString("/Game/Assets/Items/") + AssetName + FString(".") + AssetName;
	UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(PathToLoad)));
	ItemImage->SetBrushFromTexture(tmpTexture);

	//TODO: Find a more elegant sizing way!
	UpdateSizeForInventory();
}

void URPG_ItemWidget::UpdateSizeForInventory()
{
	auto Data = ItemRef->ItemInformation;
	Cast<UCanvasPanelSlot>(Slot)->SetSize(FVector2D(100.0f * Data.Width, 100.0f * Data.Height));
	Cast<UCanvasPanelSlot>(Slot)->SetPosition(FVector2D(100.f * (Data.InventoryX), 100.f * (Data.InventoryY)));
}

void URPG_ItemWidget::UpdateSizeForHUD()
{
	auto Data = ItemRef->ItemInformation;
	Cast<UCanvasPanelSlot>(Slot)->SetSize(FVector2D(100.0f * Data.Width* InventoryScale, 100.0f * Data.Height* InventoryScale));
	Cast<UCanvasPanelSlot>(Slot)->SetPosition(FVector2D(100.f * (Data.InventoryX), 100.f * (Data.InventoryY)));
}

FReply URPG_ItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bIsSelected)
	{
		auto MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
		InitOffset = Cast<UCanvasPanelSlot>(Slot)->GetPosition() - MousePos;
				
		bIsSelected = true;

		RPGEventManager->ItemWidgetPicked.Broadcast(this);
	}
	else
	{
		bIsSelected = false;
	}

	//Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return FReply::Handled();
}

void URPG_ItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = false;
}


void URPG_ItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bIsSelected)
	{
		FollowMouse();
	}
}

void URPG_ItemWidget::FollowMouse()
{
	auto MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	Cast<UCanvasPanelSlot>(Slot)->SetPosition(MousePos + FVector2D(30.0f, 30.0f)/* * UWidgetLayoutLibrary::GetViewportScale(this)*/);
}

