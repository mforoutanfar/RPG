// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameStateBase.h"
#include "RPG_EventManager.h"

ARPG_GameStateBase::ARPG_GameStateBase()
{
	EventManager = CreateDefaultSubobject<URPG_EventManager>(FName("EventManager"));
}

void ARPG_GameStateBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
