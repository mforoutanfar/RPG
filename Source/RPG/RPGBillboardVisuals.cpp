// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBillboardVisuals.h"
#include "Camera/CameraComponent.h"

URPGBillboardVisuals::URPGBillboardVisuals()
{
	//SetComponentTickEnabled(true);
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.bStartWithTickEnabled = true;
}

void URPGBillboardVisuals::BeginPlay()
{
	Super::BeginPlay();

	SetHiddenInGame(false);

	PopulateSprites();

	SetSprite(Sprites[IDLE][FRONT][0]);

	BillboardOwner = GetOwner();

	Camera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;		

	SetAnimState(WALK);
}

void URPGBillboardVisuals::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateOrientation();
}

void URPGBillboardVisuals::PopulateSprites()
{
	TArray<FString> Codes = 
	{
		"B",
		"BR",
		"R",
		"FR",
		"F",
		"FL",
		"L",
		"BL"
	};

	Sprites.Add(IDLE, {});
	Sprites.Add(WALK, {});

	//Walk Sprites
	for (size_t i = 0; i < 8; i++)//8 Directions, starting with back, going clockwise
	{
		int index = 0;
		Sprites[WALK].Add((Orientation)i, {});

		while (true)
		{
			FString AssetName = TexturePrefix + FString("_") + FString(Codes[i]) + FString("_Walk_") + FString::FromInt(index);
			FString PathToLoad = FString("/Game/Assets/CharTextures/") + TexturePrefix + "/" + AssetName + FString(".") + AssetName;

			UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(PathToLoad)));

			if (tmpTexture)
			{
				Sprites[WALK][(Orientation)i].Add(tmpTexture);
			}
			else
			{
				check(index != 0);
				break;
			}

			index++;
		}

	}

	//Idle Sprites
	for (size_t i = 0; i < 8; i++)//8 Directions, starting with back, going clockwise
	{
		int index = 0;
		Sprites[IDLE].Add((Orientation)i, {});

		while (true)
		{
			FString AssetName = TexturePrefix + FString("_") + FString(Codes[i]) + FString("_Idle_") + FString::FromInt(index);
			FString PathToLoad = FString("/Game/Assets/CharTextures/") + TexturePrefix + "/" + AssetName + FString(".") + AssetName;

			UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(PathToLoad)));

			if (tmpTexture)
			{
				Sprites[IDLE][(Orientation)i].Add(tmpTexture);
			}
			else
			{
				check(index != 0);
				break;
			}

			index++;
		}
	}
}

void URPGBillboardVisuals::SetAnimState(AnimState state)
{
	if (CurrentAnimState != state)
	{
		CurrentAnimState = state;
		CurrentFrame = 0;

		auto numberOfFrames = Sprites[CurrentAnimState][CurrentOrientation].Num();
		if (numberOfFrames > 1)//Has Animation
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URPGBillboardVisuals::AdvanceFrame, spf, true);
		}
		else
		{
			TimerHandle.Invalidate();
		}
	}
}

void URPGBillboardVisuals::AdvanceFrame()
{
	CurrentFrame++;
	if (CurrentFrame >= Sprites[CurrentAnimState][CurrentOrientation].Num())
	{
		CurrentFrame = 0;
	}
	UpdateSprite();
}

void URPGBillboardVisuals::UpdateOrientation()
{
	auto pos = BillboardOwner->GetActorLocation();
	auto camPos = Camera->GetCameraLocation();
	auto diff = camPos - pos;
	auto diffProj = FVector2D(diff.X, diff.Y);
	diffProj.Normalize();
	auto forward = BillboardOwner->GetActorForwardVector();
	auto forwardProj = FVector2D(forward.X, forward.Y);
	forwardProj.Normalize();	

	auto angle = FMath::Atan2(forwardProj.Y, forwardProj.X) - FMath::Atan2(diffProj.Y, diffProj.X);
	angle = FMath::Wrap(angle, -PI, PI);

	angle += PI + PI /8.0f;
	int eighth = int(angle / (PI / 4.0f));
	if (eighth == 8)
	{
		eighth = 0;
	}

	if (CurrentOrientation != Orientation(eighth))
	{
		CurrentOrientation = Orientation(eighth);
		UpdateSprite();
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), eighth));
}

void URPGBillboardVisuals::UpdateSprite()
{
	SetSprite(Sprites[CurrentAnimState][CurrentOrientation][CurrentFrame]);
}
