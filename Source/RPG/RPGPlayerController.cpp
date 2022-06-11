// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "RPGPlayer.h"
#include "RPG_HUD.h"
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
	ToggleInventory();

	Cast<ARPG_HUD>(GetHUD())->OnOpenInventoryPressed(InventoryOpen);	
}


void ARPGPlayerController::OnPausePressed()
{
	//Close inventory if open.
	if (InventoryOpen)
	{
		OnOpenInventoryPressed();
	}
	else
	{
		TogglePauseMenu();
		Cast<ARPG_HUD>(GetHUD())->OnPausePressed(PauseMenuOpen);
	}
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
	Cast<ARPGPlayer>(GetPawn())->OnForwardBackwardPressed(Value);
}

void ARPGPlayerController::OnStrafePressed(float Value)
{
	Cast<ARPGPlayer>(GetPawn())->OnStrafePressed(Value);
}

void ARPGPlayerController::OnLookYaw(float Value)
{
	Cast<ARPGPlayer>(GetPawn())->OnLookYaw(Value);
}

void ARPGPlayerController::OnLookPitch(float Value)
{
	Cast<ARPGPlayer>(GetPawn())->OnLookPitch(Value);
}

void ARPGPlayerController::OnJumpPressed()
{
	Cast<ARPGPlayer>(GetPawn())->OnJumpPressed();
}

void ARPGPlayerController::OnJumpReleased()
{
	Cast<ARPGPlayer>(GetPawn())->OnJumpReleased();
}

void ARPGPlayerController::OnRunPressed()
{
	Cast<ARPGPlayer>(GetPawn())->OnRunPressed();
}

void ARPGPlayerController::OnRunReleased()
{
	Cast<ARPGPlayer>(GetPawn())->OnRunReleased();
}

void ARPGPlayerController::OnInteractPressed()
{
	Cast<ARPGPlayer>(GetPawn())->OnInteractPressed();
}

void ARPGPlayerController::OnAttackPressed()
{
	Cast<ARPGPlayer>(GetPawn())->OnAttackPressed();
}

void ARPGPlayerController::OnSpellPressed()
{
	Cast<ARPGPlayer>(GetPawn())->OnSpellPressed();
}

void ARPGPlayerController::OnSwitchUnitPressed()
{
	Cast<ARPGPlayer>(GetPawn())->OnSwitchUnitPressed();
}
