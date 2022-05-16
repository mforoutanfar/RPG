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
	PAIN
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

	UFUNCTION()
	void OnInventoryItemAdded(class URPGInventoryItem* Item, class ARPGCreature* Creature);

	UFUNCTION()
	void OnAttackOccured(AActor* Attacker, AActor* Target, FRPGAttackResults Results);
		
	UFUNCTION()
	void OnSelectedUnitChanged(ARPGPlayerUnit* Unit);

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

protected:
	virtual void NativeConstruct() override;

	TWeakObjectPtr<ARPGPlayerUnit> ReferencedUnit = nullptr;

public:
	void Init(ARPGPlayerUnit* Unit);
};
