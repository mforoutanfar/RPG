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

	if (!CanvasSlot)
	{
		return;
	}

	if (!PlacedInPosition)
	{
		FollowTarget();
		FollowerVisuals->SetVisibility(ESlateVisibility::Visible);
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

	if (Target.IsValid())
	{
		auto TargetPos = Target.Get()->GetActorLocation() + DesignOffset;
		UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(Target.Get(), 0), TargetPos, PosOnScreen);
		CanvasSlot->SetPosition((PosOnScreen)/ VPScale);
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