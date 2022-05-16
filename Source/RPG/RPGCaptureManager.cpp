// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCaptureManager.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HighResScreenshot.h"
#include "Animation/AnimationAsset.h"
#include "Animation/AnimationAsset.h"
#include "Animation/AnimSingleNodeInstance.h"

/**
 * Sets default values
*/
ARPGCaptureManager::ARPGCaptureManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * Called when the game starts or when spawned
*/
void ARPGCaptureManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Camera.Get());
	ChangeAnimation();
	TakeAllShots();
}

void ARPGCaptureManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPGCaptureManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (TakingShots)
	{
		if (!DelayBetweenAngles)
		{
			auto ins = SkeletalMeshComp->GetSingleNodeInstance();
			auto length = ins->GetLength();
			SkeletalMeshComp->SetPosition(length * float(CurrentFrame) / Frames);
			TakeShot();
			CurrentFrame++;
		}

		if (CurrentFrame > Frames)
		{	
			if (BetweenAnglesCounter > 0)//delay to allow angle change
			{
				DelayBetweenAngles = true;
				BetweenAnglesCounter--;
				return;
			}
			else
			{
				DelayBetweenAngles = false;
				BetweenAnglesCounter = 10;
			}

			CurrentFrame = 1;

			if (CurrentEighth < 7)
			{
				ChangeAngle(CurrentEighth + 1);
			}
			else
			{
				ChangeAngle(0);
				if (CurrentAnimState == DIE)
				{
					TakingShots = false;
				}
				else
				{
					ChangeAnimation();
				}
			}
		}
	}
}

void ARPGCaptureManager::Init()
{
	AnimStateSuffix.Add(NONE, "ERROR");
	AnimStateSuffix.Add(IDLE, "Idle");
	AnimStateSuffix.Add(WALK, "Walk");
	AnimStateSuffix.Add(ATTACK, "Attack");
	AnimStateSuffix.Add(HIT, "Hit");
	AnimStateSuffix.Add(DIE, "Die");

	checkf(GetWorld(), TEXT("No World!"));

	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this->GetWorld(), ACameraActor::StaticClass(), FName("Camera"), CameraActors);

	checkf(CameraActors.Num() > 0, TEXT("No Camera!"));

	Camera = Cast<ACameraActor>(CameraActors[0]);

	TArray<AActor*> ModelActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this->GetWorld(), AActor::StaticClass(), FName("Model"), ModelActors);

	checkf(ModelActors.Num() > 0, TEXT("No Model!"));

	Model = ModelActors[0];

	SkeletalMeshComp = Cast<USkeletalMeshComponent>(Model->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	checkf(SkeletalMeshComp, TEXT("No SkeletalMesh!"));

	CamComponent = Cast<UCameraComponent>(Camera->GetComponentByClass(UCameraComponent::StaticClass()));
	checkf(CamComponent, TEXT("No Camera Component!"));

	CamComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	
	auto Extent = SkeletalMeshComp->Bounds.GetBox();
	SkeletalMeshComp->SetRenderCustomDepth(true);
	SkeletalMeshComp->bForceMipStreaming = true;
	SkeletalMeshComp->SetAnimation(nullptr);
	
	GScreenshotResolutionX = Resolution;
	GScreenshotResolutionY = GScreenshotResolutionX / CamComponent->AspectRatio;

	GetHighResScreenshotConfig().SetHDRCapture(true);
	GetHighResScreenshotConfig().bMaskEnabled = true;	
}

void ARPGCaptureManager::TakeShot()
{	
	int FrameNumber = CurrentFrame - 1;

	if (CurrentAnimState == DIE)//for DIE only for some reason!
	{
		FrameNumber = CurrentFrame - 2;
		if (FrameNumber < 0)
		{
			FrameNumber = 7;
		}
	}	

	FString FileName = "Corpse_" + EighthSuffix[CurrentEighth] + "_" + AnimStateSuffix[CurrentAnimState] +"_" + FString::Printf(TEXT("%d"), FrameNumber);

	GetHighResScreenshotConfig().SetFilename(FileName);
	GetWorld()->GetGameViewport()->Viewport->TakeHighResScreenShot();
}

void ARPGCaptureManager::ChangeAnimation()
{
	int CurInt = int(CurrentAnimState)+1;
	CurrentAnimState = (AnimStates)(FMath::Wrap(CurInt, 0, 5));
	UAnimationAsset* anim = AnimationMap[CurrentAnimState];	
	SkeletalMeshComp->SetAnimation(anim);
	//SkeletalMeshComp->Play(true);
}

void ARPGCaptureManager::ChangeAngle(int eighth)
{
	Model->SetActorRotation(FRotator(0.0f, eighth *45.0f,0.0f));
	CurrentEighth = eighth;
}

void ARPGCaptureManager::TakeAllShots()
{
	TakingShots = true;
}

