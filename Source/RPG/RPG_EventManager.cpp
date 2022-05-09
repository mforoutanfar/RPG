// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_EventManager.h"

TWeakObjectPtr<URPG_EventManager> URPG_EventManager::Instance(nullptr);

TWeakObjectPtr<URPG_EventManager> URPG_EventManager::GetInstance()
{
	if (!Instance.IsValid())
	{
		Instance = NewObject<URPG_EventManager>();
	}

	return Instance;
}
