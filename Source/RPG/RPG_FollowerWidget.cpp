// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_FollowerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void URPG_FollowerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FollowerVisuals->SetVisibility(ESlateVisibility::Collapsed);
}

void URPG_FollowerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//To prevent jump on first frame.
	if (ShouldBecomeVisibleNextTick)
	{
		FollowerVisuals->SetVisibility(ESlateVisibility::Visible);
		ShouldBecomeVisibleNextTick = false;
	}

	if (!CanvasSlot)
	{
		if (Slot)
		{
			if (auto CS = Cast<UCanvasPanelSlot>(Slot))
			{
				CanvasSlot = CS;
			}
		}
		return;
	}

	if (!PlacedInPosition)
	{
		FollowTarget();
		if (ActorTarget.IsValid() || WidgetTarget.IsValid() || !InvisibleWhenNoTarget)
		{
			ShouldBecomeVisibleNextTick = true;
		}		
		PlacedInPosition = true;
	}
	else if (KeepFollowing)
	{
		FollowTarget();
	}	
}

void URPG_FollowerWidget::FollowTarget()
{
	float VPScale = UWidgetLayoutLibrary::GetViewportScale(this);

	if (ActorTarget.IsValid())
	{
		auto TargetPos = ActorTarget.Get()->GetActorLocation() + DesignOffset;
		UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(this, 0), TargetPos, PosOnScreen);
		CanvasSlot->SetPosition((PosOnScreen)/ VPScale);
	}	
	else if (WidgetTarget.IsValid())
	{
		auto TargetPos = WidgetTarget.Get()->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.5f,0.5f));		
		CanvasSlot->SetPosition(TargetPos);
	}
	else if (RemoveOnTargetInvalidated)
	{
		RemoveFromParent();
	}
	else
	{
		FVector2D Result;

		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(Result);
		}

		CanvasSlot->SetPosition((Result / VPScale)/ 2.0f);
	}
}

void URPG_FollowerWidget::SetTarget(UObject* InTarget)
{
	auto NewActorTarget = Cast<AActor>(InTarget);
	auto NewWidgetTarget = Cast<UWidget>(InTarget);

	if (ActorTarget != NewActorTarget)
	{
		ActorTarget = NewActorTarget;
	}

	if (WidgetTarget != NewWidgetTarget)
	{
		WidgetTarget = NewWidgetTarget;
	}

	FollowerVisuals->SetVisibility(ESlateVisibility::Collapsed);
	PlacedInPosition = false;
}