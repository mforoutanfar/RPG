// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGFunctionLibrary.h"

#include "RPG_AvatarWidget.generated.h"

class ARPGPlayerUnit;

UENUM()
enum AvatarState
{
	NORMAL,
	ANGRY,
	SAD,
	DEAD,
	DISAPPOINTED,
	HAPPY,
	PAIN,
	COCKY
};

/**
 * 
 */
UCLASS()
class RPG_API URPG_AvatarWidget : public UUserWidget
{
	GENERATED_BODY()

	URPG_AvatarWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	class UImage* Portrait;

	UPROPERTY(meta = (BindWidget))
		class UImage* RecoveryIndicator;

	UPROPERTY(meta = (BindWidget))
		class UImage* SelectedIndicator;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ManaBar;

	UFUNCTION()
	void OnRecoveryStateChanged(AActor* Unit, bool State);

	void ResetToDefaultColor();

	UFUNCTION()
	void OnInventoryItemAdded(class URPGInventoryItem* Item, class ARPGCreature* Creature);

	UFUNCTION()
	void OnAttackOccured(AActor* Attacker, FRPGAttackData Data, FRPGAttackResults Results);
		
	UFUNCTION()
	void OnSelectedUnitChanged(ARPGPlayerUnit* Unit);

	UFUNCTION()
		void OnCreatureStateChanged(ARPGCreature* Creature);

	UFUNCTION()
	void OnSafetyStateChanged(ARPGPlayerUnit* Unit, TEnumAsByte<UnitSafety::SafetyState> State);

	UnitSafety::SafetyState DefaultState;

	void ResetAvatar();

	TMap<TEnumAsByte<AvatarState>, UTexture2D*> AvatarMap;

	FTimerHandle ResetAvatarHandle;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float ResetDelay = 0.5f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		FLinearColor SafeColor;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		FLinearColor WarningColor;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		FLinearColor DangerColor;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		FLinearColor RecoveryColor;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		FLinearColor SelectedColor;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		FLinearColor DeselectedColor;
	
	FLinearColor DefaultSafetyColor;

protected:
	virtual void NativeConstruct() override;

	TWeakObjectPtr<ARPGPlayerUnit> ReferencedUnit = nullptr;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	void Init(ARPGPlayerUnit* Unit);
};
