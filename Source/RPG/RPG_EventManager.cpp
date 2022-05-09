// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_EventManager.h"

URPG_EventManager* URPG_EventManager::Instance(nullptr);

URPG_EventManager* URPG_EventManager::GetInstance()
{
	if (!Instance)
	{
		return nullptr;
	}
	else
	{
		return Instance;
	}	
}
