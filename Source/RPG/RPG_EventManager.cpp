// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_EventManager.h"

URPG_EventManager* URPG_EventManager::Instance(nullptr);

void URPG_EventManager::BeginDestroy()
{
	Super::BeginDestroy();
	Instance = nullptr;
}

URPG_EventManager* URPG_EventManager::GetInstance()
{
	if (!Instance)
	{
		Instance = NewObject<URPG_EventManager>();
	}

	return Instance;
}
