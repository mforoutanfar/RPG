// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG_SafeAreaManager.generated.h"

/**
 *
*/
UCLASS()
class RPG_API ARPG_SafeAreaManager : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * Sets default values for this actor's properties
	*/
	ARPG_SafeAreaManager();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsPointInSafeArea(FVector Point);

protected:
	/**
	 * Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

public:	
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation 
	*/
	virtual void Tick(float DeltaTime) override;

};
