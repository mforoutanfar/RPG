// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGFunctionLibrary.h"

#include "RPGInteractable.generated.h"

using namespace InteractableCategory;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class URPGInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IRPGInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnInteracted(bool Successful);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		InteractableCategory::InteractableCat GetInteractableType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool IsInteractable();
};
