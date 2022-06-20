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
	COCKY,
	SPELL
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

	UPROPERTY(meta = (BindWidget))
		class UWidget* ManaGroup;

	UFUNCTION()
	void OnRecoveryStateChanged(AActor* Unit, bool State);

	void ResetToDefaultColor();

	UFUNCTION()
	void OnInventoryItemAdded(class URPGInventoryItem* Item, AActor* Owner);

	UFUNCTION()
	void OnInteractionOccured(AActor* Interactor, TEnumAsByte<InteractableCategory::InteractableCat> Category, bool Successful);

	UFUNCTION()
	void OnAttackOccured(AActor* Attacker, FRPGAttackData Data, FRPGAttackResults Results);
		
	UFUNCTION()
	void OnSelectedUnitChanged(ARPGPlayerUnit* Unit);

	UFUNCTION()
	void OnContainerFocusStateChanged(AActor* Container, bool IsFocused);

	UFUNCTION()
		void OnCreatureStateChanged(ARPGCreature* Creature);

	UFUNCTION()
		void OnSpellCast(ARPGCreature* Creature);

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

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void OnRightClick();
	void OnLeftClick(FKey& Button);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
		
	bool InRecovery = false;

	bool HasAddedListeners = false;

public:
	TWeakObjectPtr<AActor> ReferencedUnit = nullptr;
	void Init(AActor* Unit);
	void AddListenersOnce();
};
