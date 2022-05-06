// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGCaptureManager.generated.h"

class UAnimationAsset;

UENUM()
enum AnimStates
{
	NONE,
	IDLE,
	WALK,
	ATTACK,
	HIT,
	DIE
};

/**
 *
*/
UCLASS(Blueprintable)
class RPG_API ARPGCaptureManager : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * Sets default values for this actor's properties
	*/
	ARPGCaptureManager();

protected:
	/**
	 * Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	/**
	 * Called every frame
	 * @param DeltaTime Game time elapsed during last frame modified by the time dilation 
	*/
	virtual void Tick(float DeltaTime) override;

private:
	
	TWeakObjectPtr<AActor> Model = nullptr;

	class USkeletalMeshComponent* SkeletalMeshComp;

	TWeakObjectPtr<class ACameraActor> Camera = nullptr;

	class UCameraComponent* CamComponent;

	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<AnimStates>, UAnimationAsset*> AnimationMap;

public:
	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void TakeShot();

	UFUNCTION(BlueprintCallable)
	void ChangeAnimation();

	UFUNCTION(BlueprintCallable)
	void ChangeAngle(int angle);

	void TakeAllShots();

	UPROPERTY(EditAnywhere)
		int Frames = 8;

	int CurrentFrame = 0;
	bool TakingShots = false;
	int CurrentEighth = 0;

	TArray<FString> EighthSuffix = {"F", "FL", "L", "BL", "B", "BR", "R", "FR"};
	TMap<TEnumAsByte<AnimStates>, FString> AnimStateSuffix;

	UPROPERTY(EditAnywhere)
		float Resolution = 200.0f;

	AnimStates CurrentAnimState = WALK;
};
