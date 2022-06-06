// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBillboardVisuals.h"
#include "Camera/CameraComponent.h"
#include "RPG_EventManager.h"
#include "RPGCreature.h"
#include "RPG_GameStateBase.h"

URPGBillboardVisuals::URPGBillboardVisuals()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetHiddenInGame(false);

	ShouldLoopByDefault.Add(AnimState::AST_NONE, false);
	ShouldLoopByDefault.Add(AnimState::AST_IDLE, true);
	ShouldLoopByDefault.Add(AnimState::AST_WALK, true);
	ShouldLoopByDefault.Add(AnimState::AST_ATTACK, false);
	ShouldLoopByDefault.Add(AnimState::AST_HIT, false);
	ShouldLoopByDefault.Add(AnimState::AST_DIE, false);

	ShouldReturnToDefault.Add(AnimState::AST_NONE, true);
	ShouldReturnToDefault.Add(AnimState::AST_IDLE, false);
	ShouldReturnToDefault.Add(AnimState::AST_WALK, false);
	ShouldReturnToDefault.Add(AnimState::AST_ATTACK, true);
	ShouldReturnToDefault.Add(AnimState::AST_HIT, true);
	ShouldReturnToDefault.Add(AnimState::AST_DIE, false);

	float DefaultSPF = 0.12f;
	AnimSPF.Add(AnimState::AST_NONE, DefaultSPF);
	AnimSPF.Add(AnimState::AST_IDLE, DefaultSPF);
	AnimSPF.Add(AnimState::AST_WALK, DefaultSPF);
	AnimSPF.Add(AnimState::AST_ATTACK, DefaultSPF);
	AnimSPF.Add(AnimState::AST_HIT, DefaultSPF);
	AnimSPF.Add(AnimState::AST_DIE, DefaultSPF);
}


void URPGBillboardVisuals::Init(FString texturePrefix)//Called by Owner
{
	TexturePrefix = texturePrefix;
	PopulateSprites();	
	RPGEventManager->AttackOccured.AddDynamic(this, &URPGBillboardVisuals::OnAttackOccured);
}


void URPGBillboardVisuals::BeginPlay()
{
	Super::BeginPlay();

	Camera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	//SetAnimState(WALK);
}

void URPGBillboardVisuals::OnOwnerWalkingStateChanged(ARPGCreature* Creature, bool State)
{
	if (State)
	{
		if (DefaultAnimState != AnimState::AST_WALK)
		{
			SetDefaultAnimState(AnimState::AST_WALK);

			if (CurrentAnimState == AnimState::AST_IDLE || CurrentAnimState == AnimState::AST_NONE)//TODO: Any other case?
			{
				SetAnimState(AnimState::AST_WALK);
			}			
		}
	}
	else
	{
		SetDefaultAnimState(AnimState::AST_IDLE);
		if (CurrentAnimState == AnimState::AST_WALK || CurrentAnimState == AnimState::AST_NONE)//TODO: Any other case?
		{
			SetAnimState(AnimState::AST_IDLE);
		}		
	}
}

void URPGBillboardVisuals::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!CurrentAnimState == AnimState::AST_NONE)
	{
		UpdateOrientation();
	}
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
	FString string;
	switch (state)
	{
	case AnimationState::AnimState::AST_NONE:
	{
		string = "NONE";
		break;
	}	
	case AnimationState::AnimState::AST_IDLE:
	{
		string = "IDLE";
		break;
	}
	case AnimationState::AnimState::AST_WALK:
	{
		string = "WALK";
		break;
	}
	case AnimationState::AnimState::AST_ATTACK:
	{
		string = "ATTACK";
		break;
	}
	case AnimationState::AnimState::AST_HIT:
	{
		string = "HIT";
		break;
	}
	case AnimationState::AnimState::AST_DIE:
	{
		string = "DIE";
		break;
	}
	default:
		break;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, string);

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

void URPGBillboardVisuals::SetDefaultAnimState(AnimState state)
{
	DefaultAnimState = state;
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
				SetAnimState(DefaultAnimState);
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				CurrentFrame = Sprites[CurrentAnimState][CurrentOrientation].Num()-1;
			}
		}		
	}
	UpdateSprite();

	//TODO: Cleaner system for animation callbacks?
	if (CurrentAnimState == AnimState::AST_ATTACK && CurrentFrame == AttackCallbackFrame)
	{
		AttackCallback.Execute();
	}
}

void URPGBillboardVisuals::UpdateOrientation()
{
	auto pos = GetOwner()->GetActorLocation();
	auto camPos = Camera->GetCameraLocation();
	auto diff = camPos - pos;
	auto diffProj = FVector2D(diff.X, diff.Y);
	diffProj.Normalize();
	auto forward = GetOwner()->GetActorForwardVector();
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
}

void URPGBillboardVisuals::UpdateSprite()
{
	SetSprite(Sprites[CurrentAnimState][CurrentOrientation][CurrentFrame]);
}

void URPGBillboardVisuals::OnOwnerDied()
{
	SetAnimState(AnimState::AST_DIE);
}

void URPGBillboardVisuals::BeginAttack()
{
	SetAnimState(AnimState::AST_ATTACK);
}

void URPGBillboardVisuals::OnAttackOccured(AActor* Attacker, FRPGAttackData Data, FRPGAttackResults Results)
{	
	if (Data.Target == GetOwner())
	{
		if (Results.TargetDied)
		{
			SetAnimState(AnimState::AST_DIE);
		}
		else if (!Results.Missed)
		{
			SetAnimState(AnimState::AST_HIT);
		}		
	}
}
