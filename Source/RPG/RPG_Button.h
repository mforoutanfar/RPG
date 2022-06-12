// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include  "RPG\RPGInteractable.h"

#include "RPG_Button.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRPGButtonPressed, AActor*, Button);

/**
 *
*/
UCLASS()
class RPG_API ARPG_Button : public AActor, public IRPGInteractable
{
	GENERATED_BODY()
	
public:	
	/**
	 * Sets default values for this actor's properties
	*/
	ARPG_Button();

	virtual void OnInteracted(bool Successful) override;

	//TODO: Implement Button Group (multiple buttons connected to same functionality).
	UFUNCTION(BlueprintCallable)
	void SetIsPressed(bool InIsPressed);

	virtual bool IsInteractable() override;

	virtual InteractableCat GetInteractableType() override;

protected:
	/**
	 * Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

	bool IsPressed = false;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* HitBox;
	
public:	
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation 
	*/
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnRPGButtonFunctionFinished();	

	UPROPERTY(BlueprintAssignable)
	FRPGButtonPressed RPGButtonPressed;
};
