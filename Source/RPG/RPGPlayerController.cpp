// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "RPGPlayer.h"
#include "RPG_HUD.h"
#include "RPG_GameHUD.h"
#include "RPG_GameStateBase.h"
#include "RPG_EventManager.h"

#include "Kismet/GameplayStatics.h"

void ARPGPlayerController::SetupInputComponent()
{
    // Always call this.
    Super::SetupInputComponent();

	InputComponent->BindAxis("Move FB", this, &ARPGPlayerController::OnForwardBackwardPressed);
	InputComponent->BindAxis("Strafe", this, &ARPGPlayerController::OnStrafePressed);
	InputComponent->BindAxis("Look Pitch", this, &ARPGPlayerController::OnLookPitch);
	InputComponent->BindAxis("Look Yaw", this, &ARPGPlayerController::OnLookYaw);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ARPGPlayerController::OnJumpPressed);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ARPGPlayerController::OnJumpReleased);
	InputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ARPGPlayerController::OnRunPressed);
	InputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ARPGPlayerController::OnRunReleased);
	InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ARPGPlayerController::OnInteractPressed);
	InputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ARPGPlayerController::OnAttackPressed);	
	InputComponent->BindAction("Spell", EInputEvent::IE_Pressed, this, &ARPGPlayerController::OnSpellPressed);

	auto& Binding1 = InputComponent->BindAction("SwitchUnit", EInputEvent::IE_Pressed, this, &ARPGPlayerController::OnSwitchUnitPressed);
	Binding1.bExecuteWhenPaused = true;
	auto &Binding2 = InputComponent->BindAction("Open Inventory", EInputEvent::IE_Pressed, this, &ARPGPlayerController::OnOpenInventoryPressed);
	Binding2.bExecuteWhenPaused = true;	
	auto& Binding3 = InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &ARPGPlayerController::OnPausePressed);
	Binding3.bExecuteWhenPaused = true;
}

void ARPGPlayerController::OnOpenInventoryPressed()
{
	if (GameOver)
	{
		return;
	}

	if (EventOpen)
	{
		return;
	}

	ToggleInventory();

	RPGGameHUD->OnOpenInventoryPressed(InventoryOpen);	
}


void ARPGPlayerController::OnPausePressed()
{
	if (GameOver)
	{
		return;
	}

	if (EventOpen)
	{
		CloseStoryEvent();
	}
	else if (InventoryOpen)
	{
		OnOpenInventoryPressed();
	}
	else
	{
		TogglePauseMenu();
		RPGGameHUD->OnPausePressed(PauseMenuOpen);
	}
}

void ARPGPlayerController::OnGameOverIssued()
{
	GameOver = true;

	auto InputMode = FInputModeUIOnly();
	SetInputMode(InputMode);

	SetShowMouseCursor(true);
}

void ARPGPlayerController::OnStoryEventTriggered(FString EventName)
{
	SetShowMouseCursor(true);

	auto InputMode = FInputModeGameAndUI();
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	RPGGameHUD->OnStoryEventTriggered(EventName);

	EventOpen = true;

	SetPause(true);
}

void ARPGPlayerController::CloseStoryEvent()
{
	EventOpen = false;

	SetShowMouseCursor(false);

	SetInputMode(FInputModeGameOnly());

	RPGGameHUD->CloseStoryEvent();

	SetPause(false);
}


void ARPGPlayerController::BeginPlay()
{
	RPGEventManager->GameOverIssued.AddDynamic(this, &ARPGPlayerController::OnGameOverIssued);
	RPGEventManager->StoryEventTriggered.AddDynamic(this, &ARPGPlayerController::OnStoryEventTriggered);
}

void ARPGPlayerController::ToggleInventory()
{
	InventoryOpen = !InventoryOpen;

	SetShowMouseCursor(InventoryOpen);

	if (InventoryOpen)
	{
		auto InputMode = FInputModeGameAndUI();
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}

	SetPause(InventoryOpen);
}

void ARPGPlayerController::TogglePauseMenu()
{
	PauseMenuOpen = !PauseMenuOpen;

	SetShowMouseCursor(PauseMenuOpen);

	if (PauseMenuOpen)
	{
		auto InputMode = FInputModeGameAndUI();
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}

	SetPause(PauseMenuOpen);
}

void ARPGPlayerController::OnForwardBackwardPressed(float Value)
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnForwardBackwardPressed(Value);
}

void ARPGPlayerController::OnStrafePressed(float Value)
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnStrafePressed(Value);
}

void ARPGPlayerController::OnLookYaw(float Value)
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnLookYaw(Value);
}

void ARPGPlayerController::OnLookPitch(float Value)
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnLookPitch(Value);
}

void ARPGPlayerController::OnJumpPressed()
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnJumpPressed();
}

void ARPGPlayerController::OnJumpReleased()
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnJumpReleased();
}

void ARPGPlayerController::OnRunPressed()
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnRunPressed();
}

void ARPGPlayerController::OnRunReleased()
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnRunReleased();
}

void ARPGPlayerController::OnInteractPressed()
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnInteractPressed();
}

void ARPGPlayerController::OnAttackPressed()
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnAttackPressed();
}

void ARPGPlayerController::OnSpellPressed()
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnSpellPressed();
}

void ARPGPlayerController::OnSwitchUnitPressed()
{
	if (GameOver)
	{
		return;
	}

	Cast<ARPGPlayer>(GetPawn())->OnSwitchUnitPressed();
}
