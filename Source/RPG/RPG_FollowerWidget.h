// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG_FollowerWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_FollowerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void FollowTarget();

	FVector2D PosOnScreen;

	virtual void NativeConstruct() override;

	//UPROPERTY(meta = (BindWidget))
	//class UCanvasPanel* Canvas = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget* FollowerVisuals;

	bool PlacedInPosition = false;

	UPROPERTY(EditDefaultsOnly)
	bool InvisibleWhenNoTarget = false;

	bool ShouldBecomeVisibleNextTick = false;

	TWeakObjectPtr<AActor> ActorTarget = nullptr;
	TWeakObjectPtr<UWidget> WidgetTarget = nullptr;
public:

	UPROPERTY()
		class UCanvasPanelSlot* CanvasSlot = nullptr;

	void SetTarget(UObject* InTarget);	

	UPROPERTY(EditDefaultsOnly)
	bool KeepFollowing = false;

	UPROPERTY(EditDefaultsOnly)
	bool RemoveOnTargetInvalidated = false;

	UPROPERTY(EditDefaultsOnly)
		FVector DesignOffset;
};
