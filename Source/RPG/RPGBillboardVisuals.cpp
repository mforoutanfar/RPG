// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBillboardVisuals.h"
#include "Camera/CameraComponent.h"

URPGBillboardVisuals::URPGBillboardVisuals()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetHiddenInGame(false);

	ShouldLoopByDefault.Add(NONE, false);
	ShouldLoopByDefault.Add(IDLE, true);
	ShouldLoopByDefault.Add(WALK, true);
	ShouldLoopByDefault.Add(ATTACK, false);
	ShouldLoopByDefault.Add(HIT, false);
	ShouldLoopByDefault.Add(DIE, false);

	ShouldReturnToDefault.Add(NONE, true);
	ShouldReturnToDefault.Add(IDLE, false);
	ShouldReturnToDefault.Add(WALK, false);
	ShouldReturnToDefault.Add(ATTACK, true);
	ShouldReturnToDefault.Add(HIT, true);
	ShouldReturnToDefault.Add(DIE, false);

	float DefaultSPF = 0.12f;
	AnimSPF.Add(NONE, DefaultSPF);
	AnimSPF.Add(IDLE, DefaultSPF);
	AnimSPF.Add(WALK, DefaultSPF);
	AnimSPF.Add(ATTACK, DefaultSPF);
	AnimSPF.Add(HIT, DefaultSPF);
	AnimSPF.Add(DIE, DefaultSPF);
}


void URPGBillboardVisuals::Init(FString texturePrefix)//Called by Owner
{
	TexturePrefix = texturePrefix;
	PopulateSprites();
	BillboardOwner = GetOwner();
}


void URPGBillboardVisuals::BeginPlay()
{
	Super::BeginPlay();

	Camera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
}

void URPGBillboardVisuals::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CurrentAnimState == NONE)
	{
		return;
	}

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

	TArray<FString> StateCodes =
	{
		"NONE",
		"Idle",
		"Walk",
		"ATTACK",
		"HIT",
		"DIE",
	};

	for (size_t i = 1; i <= 5; i++)//5 animation states
	{		
		AnimState State = AnimState(i);
		Sprites.Add(State, {});

		//Walk Sprites
		for (size_t j = 0; j < 8; j++)//8 Directions, starting with back, going clockwise
		{
			Orientation Orien = (Orientation)j;
			int index = 0;
			Sprites[State].Add(Orien, {});

			while (true)
			{
				FString AssetName = TexturePrefix + FString("_") + FString(Codes[j]) + FString("_") + StateCodes[i] + FString("_") + FString::FromInt(index);
				FString PathToLoad = FString("/Game/Assets/CharTextures/") + TexturePrefix + "/" + AssetName + FString(".") + AssetName;

				UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(PathToLoad)));

				if (tmpTexture)
				{
					Sprites[State][Orien].Add(tmpTexture);
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
}

void URPGBillboardVisuals::SetAnimState(AnimState state)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	CurrentAnimState = state;
	CurrentFrame = -1;
	AdvanceFrame();

	auto numberOfFrames = Sprites[CurrentAnimState][CurrentOrientation].Num();
	if (numberOfFrames > 1)//Has Animation
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URPGBillboardVisuals::AdvanceFrame, AnimSPF[CurrentAnimState], true);
	}
}

void URPGBillboardVisuals::AdvanceFrame()
{
	CurrentFrame++;
	if (CurrentFrame >= Sprites[CurrentAnimState][CurrentOrientation].Num())
	{
		CurrentFrame = 0;

		if (!ShouldLoopByDefault[CurrentAnimState])
		{
			if (ShouldReturnToDefault[CurrentAnimState])
			{
				SetAnimState(IDLE);
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				CurrentFrame = Sprites[CurrentAnimState][CurrentOrientation].Num()-1;
			}
		}		
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

void URPGBillboardVisuals::OnOwnerAttacked()
{
	SetAnimState(HIT);
}

void URPGBillboardVisuals::OnOwnerDied()
{
	SetAnimState(DIE);
}