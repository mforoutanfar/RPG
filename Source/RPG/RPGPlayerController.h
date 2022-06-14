// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	void OnOpenInventoryPressed();

	void ToggleInventory();

	void TogglePauseMenu();

	//Made blueprint callable for access from pause menu. TODO: Cleaner solution?
	UFUNCTION(BlueprintCallable)
		void OnPausePressed();

	UFUNCTION()
		void OnGameOverIssued();

	UFUNCTION()
		void OnStoryEventTriggered(FString EventName);

	void CloseStoryEvent();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool GameOver = false;

protected:
	void OnForwardBackwardPressed(float Value);
	void OnStrafePressed(float Value);
	void OnLookYaw(float Value);
	void OnLookPitch(float Value);
	void OnJumpPressed();
	void OnJumpReleased();
	void OnRunPressed();
	void OnRunReleased();
	void OnInteractPressed();
	void OnAttackPressed();
	void OnSpellPressed();
	void OnSwitchUnitPressed();

	bool InventoryOpen = false;
	bool PauseMenuOpen = false;
	bool EventOpen = false;
};

