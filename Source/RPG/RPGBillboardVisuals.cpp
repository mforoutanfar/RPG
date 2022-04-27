// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBillboardVisuals.h"
#include "Camera/CameraComponent.h"

URPGBillboardVisuals::URPGBillboardVisuals()
{
	SetComponentTickEnabled(true);
	PrimaryComponentTick.bCanEverTick = true;
}

void URPGBillboardVisuals::BeginPlay()
{
	Super::BeginPlay();

	SetHiddenInGame(false);

	PopulateSprites();

	SetSprite(IdleSprites[FRONT][0]);

	BillboardOwner = GetOwner();

	Camera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;		
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

	//Walk Sprites
	for (size_t i = 0; i < 8; i++)//8 Directions, starting with back, going clockwise
	{
		int index = 0;
		WalkSprites.Add((Orientation)i, {});

		while (true)
		{
			FString AssetName = TexturePrefix + FString("_") + FString(Codes[i]) + FString("_Walk_") + FString::FromInt(index);
			FString PathToLoad = FString("/Game/Assets/CharTextures/") + AssetName + FString(".") + AssetName;

			UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(PathToLoad)));

			if (tmpTexture)
			{
				WalkSprites[(Orientation)i].Add(tmpTexture);
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
		IdleSprites.Add((Orientation)i, {});

		while (true)
		{
			FString AssetName = TexturePrefix + FString("_") + FString(Codes[i]) + FString("_Idle_") + FString::FromInt(index);
			FString PathToLoad = FString("/Game/Assets/CharTextures/") + AssetName + FString(".") + AssetName;

			UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(PathToLoad)));

			if (tmpTexture)
			{
				IdleSprites[(Orientation)i].Add(tmpTexture);
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
	int octet = int(angle / (PI / 4.0f));
	if (octet == 8)
	{
		octet = 0;
	}

	SetSprite(IdleSprites[Orientation(octet)][0]);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), FMath::Wrap(8, 0, 7)));
}
