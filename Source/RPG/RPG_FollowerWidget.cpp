// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_FollowerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void URPG_FollowerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CanvasSlot = Cast<UCanvasPanelSlot>(FollowerVisuals->Slot);
	FollowTarget();
}

void URPG_FollowerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (KeepFollowing)
	{
		FollowTarget();
	}
}

void URPG_FollowerWidget::FollowTarget()
{
	if (Target.IsValid())
	{
		auto TargetPos = Target.Get()->GetActorLocation();
		UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(Target.Get(), 0), TargetPos, PosOnScreen);
		CanvasSlot->SetPosition((PosOnScreen + DesignOffset)/UWidgetLayoutLibrary::GetViewportScale(this));
	}	
	else if (RemoveOnTargetInvalidated)
	{
		RemoveFromParent();
	}
}