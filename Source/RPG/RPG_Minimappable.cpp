// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Minimappable.h"
#include "RPG_MiniMapWidget.h"

// Add default functionality here for any IRPG_Minimappable functions that are not pure virtual.

void IRPG_Minimappable::RegisterOnMiniMap(AActor* Actor, MiniMap::ObjectType Type)
{
	URPG_MiniMapWidget::Register(Actor, Type);
}

void IRPG_Minimappable::UnregisterFromMiniMap(AActor* Actor)
{
	URPG_MiniMapWidget::Unregister(Actor);
}
