// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_MiniMapWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "RPGPingWidget.h"

void URPG_MiniMapWidget::RegisterOnMinimap(AActor* Actor, TEnumAsByte<MiniMap::ObjectType> Type)
{
	if (!ActorMap.Find(Actor))
	{
		AddPing(Actor, Type);
	}
}

void URPG_MiniMapWidget::UnregisterFromMinimap(AActor* Actor)
{
	auto Ping = ActorMap.FindAndRemoveChecked(Actor);
	Ping->RemoveFromParent();
}

void URPG_MiniMapWidget::AddPing(AActor* Actor, MiniMap::ObjectType Type)
{
	TSubclassOf<UUserWidget> PingClass;
	FLinearColor Color;

	switch (Type)
	{
	case MiniMap::ENEMY:
	{
		PingClass = EnemyPingClass;
		break;
	}
	case MiniMap::LOOT:
	{
		PingClass = LootPingClass;
		break;
	}
	case MiniMap::NPC:
	{
		PingClass = NPCPingClass;
		break;
	}
	default:
		break;
	}

	auto Ping = WidgetTree->ConstructWidget<URPGPingWidget>(PingClass, FName(FString::Printf(TEXT("Ping%d"), ActorMap.Num())));
	PingCanvas->AddChildToCanvas(Ping);
	auto CanvasSlot = Cast<UCanvasPanelSlot>(Ping->Slot);
	auto Anchors = FAnchors();
	Anchors.Minimum = FVector2D(0.5f, 0.5f);
	Anchors.Maximum = FVector2D(0.5f, 0.5f);
	CanvasSlot->SetAnchors(Anchors);
	CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	CanvasSlot->SetAutoSize(true);

	ActorMap.Add(Actor,Ping);
}


//TODO: Move to URPGPingWidget?
void URPG_MiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	for (auto i: ActorMap)
	{
		if (i.Key)
		{			
			auto ActorPos = i.Key->GetActorLocation();
			auto PlayerPos = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
			auto PlayerRot = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorRotation();
			auto Angle = PlayerRot.Yaw;

			auto Delta = ActorPos - PlayerPos;
			auto ProjDelta = FVector2D(Delta.X, Delta.Y).GetRotated(270.0f - Angle);
			auto Position = (400.0f / 10000.0f) * ProjDelta;//400 : Width of Widget, 10000: Width of Ortho Camera. TODO: Automate!

			float MaxRadius = 165.0f;

			if (Position.SizeSquared() > MaxRadius* MaxRadius)
			{
				Position *= (MaxRadius / Position.Size());

				//TODO: should change only once
				i.Value->ChangeToPointer();
				i.Value->SetRenderTransformAngle(FMath::RadiansToDegrees(FMath::Atan2(Position.Y, Position.X)) + 90.0f);
			}
			else
			{
				//TODO: should change only once
				i.Value->ChangeToDefault();
			}
			
			Cast<UCanvasPanelSlot>(i.Value->Slot)->SetPosition(Position);
		}
	}
}
