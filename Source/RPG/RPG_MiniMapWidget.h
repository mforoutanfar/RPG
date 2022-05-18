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
	static void Register(AActor* Actor, MiniMap::ObjectType Type);
	static void Unregister(AActor* Actor);

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class URPGPingWidget> EnemyPingClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class URPGPingWidget> LootPingClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class URPGPingWidget> NPCPingClass;

	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* PingCanvas;

protected:

	static TMap<AActor*, TEnumAsByte<MiniMap::ObjectType>> RegisterMap;
	static TMap<AActor*, class URPGPingWidget*> ActorMap;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	static URPG_MiniMapWidget* Instance;

	void AddPing(AActor* Actor, MiniMap::ObjectType Type);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
