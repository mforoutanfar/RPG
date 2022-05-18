// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGFunctionLibrary.h"

#include "RPG_MiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_MiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class URPGPingWidget> EnemyPingClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class URPGPingWidget> LootPingClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class URPGPingWidget> NPCPingClass;

	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* PingCanvas;

	void RegisterOnMinimap(AActor* Actor, TEnumAsByte<MiniMap::ObjectType> Type);
	void UnregisterFromMinimap(AActor* Actor);

protected:

	void AddPing(AActor* Actor, MiniMap::ObjectType Type);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TMap<AActor*, class URPGPingWidget*> ActorMap;
};
