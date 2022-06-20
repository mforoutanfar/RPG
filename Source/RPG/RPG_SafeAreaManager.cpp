// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_SafeAreaManager.h"

/**
 * Sets default values
*/
ARPG_SafeAreaManager::ARPG_SafeAreaManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool ARPG_SafeAreaManager::IsPointInSafeArea_Implementation(FVector Point)
{
	return false;
}

/**
 * Called when the game starts or when spawned
*/
void ARPG_SafeAreaManager::BeginPlay()
{
	Super::BeginPlay();
	

}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPG_SafeAreaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

