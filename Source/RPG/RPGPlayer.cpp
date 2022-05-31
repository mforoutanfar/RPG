// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayer.h"

#include "GameFramework\CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

#include "RPGAttackable.h"
#include "RPGInteractable.h"

#include "RPGPlayerUnit.h"
#include "RPGRandomAudioComponent.h"
#include "RPG_EventManager.h"

#include "Engine\SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

#include "RPG_GameStateBase.h"

// Sets default values
ARPGPlayer::ARPGPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->DestroyComponent();
	GetMesh()->SetActive(false);

	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCameraComponent->bUsePawnControlRotation = true;	
	PlayerCameraComponent->bLockToHmd = false;

	InteractionCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionCollider"));
	InteractionCollider->SetCollisionProfileName(FName("Interactor"));

	AudioComponent = CreateDefaultSubobject<URPGRandomAudioComponent>(FName("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	SightSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("SightSource"));
	SightSource->RegisterForSense(UAISense_Sight::StaticClass());
}

void ARPGPlayer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	PlayerCameraComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	InteractionCollider->AttachToComponent(PlayerCameraComponent, FAttachmentTransformRules::KeepRelativeTransform);	
}

//TODO: Get Unit Info as Input
void ARPGPlayer::AddUnit()
{
	if (Units.Num() == UnitCapacity)
	{
		//Unit Capactiy Full!
		return;
	}

	float UnitOffset = 30.0f;

	auto Name = FString::Printf(TEXT("UnitPlaceHolder%d"), Units.Num());
	auto UnitPlaceHolder = NewObject<UChildActorComponent>(this);	
	auto posY = -(StarterUnits - 1) * UnitOffset / 2.0f + Units.Num() * UnitOffset;
	UnitPlaceHolder->SetRelativeLocation(FVector(0.0f, posY, 0.0f));
	UnitPlaceHolder->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	UnitPlaceHolder->SetChildActorClass(UnitClass);
	UnitPlaceHolder->CreateChildActor();
	UnitPlaceHolder->RegisterComponent();

	///////////////////////
	auto Unit = Cast<ARPGPlayerUnit>(UnitPlaceHolder->GetChildActor());
	Unit->UnitIndex = Units.Num();
	Units.Add(Unit);

	RPGEventManager->UnitAdded.Broadcast(Unit);
}

// Called when the game starts or when spawned
void ARPGPlayer::BeginPlay()
{
	Super::BeginPlay();	

	RPGEventManager->RecoveryStateChanged.AddDynamic(this, &ARPGPlayer::OnUnitRecoveryStateChanged);
	RPGEventManager->AvatarLeftClicked.AddDynamic(this, &ARPGPlayer::OnUnitAvatarLeftClicked);

	MiniMapCamera = GetWorld()->SpawnActor<ASceneCapture2D>(ASceneCapture2D::StaticClass());
	MiniMapCamera->SetActorLocation(GetActorLocation() + FVector(0.0f,0.0f,10000.0f));
	MiniMapCamera->GetCaptureComponent2D()->ProjectionType = ECameraProjectionMode::Orthographic;
	MiniMapCamera->GetCaptureComponent2D()->OrthoWidth = 10000.f;
	MiniMapCamera->SetActorRotation((GetActorLocation() - MiniMapCamera->GetActorLocation()).ToOrientationRotator());	

	FString PathToLoad = FString("TextureRenderTarget2D'/Game/HUD/MiniMapTexture.MiniMapTexture'");
	UTextureRenderTarget2D* tmpTexture = Cast<UTextureRenderTarget2D>(StaticLoadObject(UTextureRenderTarget2D::StaticClass(), NULL, *(PathToLoad)));

	if (tmpTexture)
	{
		MiniMapCamera->GetCaptureComponent2D()->TextureTarget = tmpTexture;
	}

	auto Rules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	MiniMapCamera->AttachToActor(this, Rules);

	for (int i = 0; i < StarterUnits; i++)
	{
		AddUnit();
		SetSelectedUnit(FindFirstOutOfRecoveryUnit());
	}
}

// Called every frame
void ARPGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARPGPlayer::OnForwardBackwardPressed(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ARPGPlayer::OnStrafePressed(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ARPGPlayer::OnLookYaw(float Value)
{
	AddControllerYawInput(Value);
}

void ARPGPlayer::OnLookPitch(float Value)
{
	AddControllerPitchInput(Value);
}

void ARPGPlayer::OnJumpPressed()
{
	Jump();
}

void ARPGPlayer::OnJumpReleased()
{
	StopJumping();
}

void ARPGPlayer::OnRunPressed()
{
	GetCharacterMovement()->MaxWalkSpeed *= 3.0f;
}

void ARPGPlayer::OnRunReleased()
{
	GetCharacterMovement()->MaxWalkSpeed /= 3.0f;
}

void ARPGPlayer::OnInteractPressed()
{
	if (auto InteractTarget = GetNearestTarget(InteractionCollider))
	{
		if (SelectedUnit.IsValid())
		{
			SelectedUnit->InteractWithTarget(InteractTarget);
		}
		else
		{
			auto Interactable = Cast<IRPGInteractable>(InteractTarget);
			auto Type = Interactable->GetInteractableType();

			if (Type == ITEM)
			{
				Units[0]->InteractWithTarget(InteractTarget);
			}
		}
	}	
}

void ARPGPlayer::OnAttackPressed()
{
	ARPGPlayerUnit* UnitToAttack = nullptr;

	if (SelectedUnit.IsValid())
	{
		if (!SelectedUnit->IsInRecovery())
		{
			UnitToAttack = SelectedUnit.Get();
		}
		else
		{
			UnitToAttack = FindFirstOutOfRecoveryUnit();
			SetSelectedUnit(UnitToAttack);
		}
	}
	else
	{
		UnitToAttack = FindFirstOutOfRecoveryUnit();
		SetSelectedUnit(UnitToAttack);
	}

	if (UnitToAttack)
	{
		UnitToAttack->Attack();
	}
}

void ARPGPlayer::OnSwitchUnitPressed()
{
	if (!SelectedUnit.IsValid())
	{
		SetSelectedUnit(Units[0]);
	}
	else
	{
		int NextUnitIndex = SelectedUnit->UnitIndex + 1;
		if (NextUnitIndex > Units.Num()-1)
		{
			NextUnitIndex = 0;
		}
		SetSelectedUnit(Units[NextUnitIndex]);
	}
}

void ARPGPlayer::OnUnitAvatarLeftClicked(ARPGPlayerUnit* Unit)
{
	SetSelectedUnit(Unit);
}

bool ARPGPlayer::CanGenerallyInteractWithTarget(IRPGInteractable* Target)
{
	auto Type = Target->GetInteractableType();
	switch (Type)
	{
	case InteractableCategory::MISC:
	case InteractableCategory::NONE:
	case InteractableCategory::ITEM:
	{
		return false;
		break;
	}
	case InteractableCategory::DOOR:
	case InteractableCategory::CHEST:
	case InteractableCategory::CORPSE:
	{
		return true;
		break;
	}
	default:
	{
		return false;
		break;
	}
	}
}

AActor* ARPGPlayer::GetNearestTarget(UShapeComponent* Collider, bool ShouldBeVisible)
{
	AActor* ClosestAttackableActor = nullptr;
	float MinDistance = FLT_MAX;
	TSet<AActor*> OverlappingActors = {};
	Collider->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (ShouldBeVisible)
		{
			if (!Actor->WasRecentlyRendered(KINDA_SMALL_NUMBER))
			{
				continue;
			}
		}

		auto Distance = (Actor->GetActorLocation() - GetActorLocation()).SizeSquared();
		if (Distance < MinDistance)
		{
			ClosestAttackableActor = Actor;
			MinDistance = Distance;
		}
	}

	return ClosestAttackableActor;
}

void ARPGPlayer::OnUnitRecoveryStateChanged(AActor* Unit, bool IsInRecovery)
{
	if (!Cast<ARPGPlayerUnit>(Unit))
	{
		return;
	}

	if (IsInRecovery)
	{
		//Set next active unit if available
		if (SelectedUnit == Unit)
		{
			ARPGPlayerUnit* NextAvailableUnit = nullptr;

			for (size_t i = 1; i < Units.Num(); i++)
			{
				int index = Cast<ARPGPlayerUnit>(Unit)->UnitIndex + i;
				if (index > Units.Num()-1)
				{
					index = 0;
				}

				if (!Units[index]->IsInRecovery())
				{
					NextAvailableUnit = Units[index];
					break;
				}
			}

			SetSelectedUnit(NextAvailableUnit);
		}
	}
	else
	{
		if (!SelectedUnit.IsValid())
		{
			SetSelectedUnit(Cast<ARPGPlayerUnit>(Unit));
		}
	}
}

ARPGPlayerUnit* ARPGPlayer::FindFirstOutOfRecoveryUnit()
{
	for (auto Unit : Units)
	{
		if (!Unit->IsInRecovery())
		{
			return Unit;
		}
	}

	return nullptr;
}

void ARPGPlayer::SetSelectedUnit(ARPGPlayerUnit* Unit)
{
	SelectedUnit = Unit;
	RPGEventManager->SelectedUnitChanged.Broadcast(Unit);
}