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

	MeleeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeBox"));
	MeleeBox->SetCollisionProfileName(FName("PlayerDamageSource"));

	RangedSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangedSphere"));
	RangedSphere->SetCollisionProfileName(FName("PlayerDamageSource"));

	AudioComponent = CreateDefaultSubobject<URPGRandomAudioComponent>(FName("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	SightSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("SightSource"));
	SightSource->RegisterForSense(UAISense_Sight::StaticClass());

	float UnitOffset = 30.0f;

	for (size_t i = 0; i < UnitCapacity; i++)
	{
		auto Name = FString::Printf(TEXT("UnitPlaceHolder%d"), i);
		auto UnitPlaceHolder = CreateDefaultSubobject<UChildActorComponent>(FName(*Name));		
		auto posY = -(UnitCapacity - 1) * UnitOffset / 2.0f + i * UnitOffset;
		UnitPlaceHolder->SetRelativeLocation(FVector(0.0f, posY, 0.0f));
		UnitPlaceHolders.Add(UnitPlaceHolder);
	}
}

void ARPGPlayer::OnConstruction(const FTransform& Transform)
{
	PlayerCameraComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	InteractionCollider->AttachToComponent(PlayerCameraComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MeleeBox->AttachToComponent(PlayerCameraComponent, FAttachmentTransformRules::KeepRelativeTransform);
	RangedSphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	for (auto Holder : UnitPlaceHolders)
	{
		Holder->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		Holder->SetChildActorClass(UnitClass);
	}	
}

//TODO: Get Unit Info as Input
void ARPGPlayer::AddUnit()
{
	if (Units.Num() == UnitCapacity)
	{
		//Unit Capactiy Full!
		return;
	}

	auto Holder = UnitPlaceHolders[Units.Num()];
	Holder->CreateChildActor();

	auto Unit = Cast<ARPGPlayerUnit>(Holder->GetChildActor());
	Unit->UnitIndex = Units.Num();
	URPG_EventManager::GetInstance()->RecoveryStateChanged.AddUObject(this, &ARPGPlayer::OnUnitRecoveryStateChanged);
	Units.Add(Unit);

	URPG_EventManager::GetInstance()->UnitAdded.Broadcast(Unit);
}

// Called when the game starts or when spawned
void ARPGPlayer::BeginPlay()
{
	Super::BeginPlay();	

	for (int i = 0; i < UnitCapacity; i++)
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

// Called to bind functionality to input
void ARPGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Move FB", this, &ARPGPlayer::OnForwardBackwardPressed);
	PlayerInputComponent->BindAxis("Strafe", this, &ARPGPlayer::OnStrafePressed);
	PlayerInputComponent->BindAxis("Look Pitch", this, &ARPGPlayer::OnLookPitch);
	PlayerInputComponent->BindAxis("Look Yaw", this, &ARPGPlayer::OnLookYaw);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed,this, &ARPGPlayer::OnJumpPressed);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ARPGPlayer::OnJumpReleased);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ARPGPlayer::OnRunPressed);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ARPGPlayer::OnRunReleased);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ARPGPlayer::OnInteractPressed);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ARPGPlayer::OnAttackPressed);
	PlayerInputComponent->BindAction("SwitchUnit", EInputEvent::IE_Pressed, this, &ARPGPlayer::OnSwitchUnitPressed);
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

void ARPGPlayer::OnAttackPressed()
{
	if (SelectedUnit.IsValid())
	{
		auto UnitToAttack = SelectedUnit;

		if (SelectedUnit->IsInRecovery())
		{
			UnitToAttack = FindFirstOutOfRecoveryUnit();
			SetSelectedUnit(UnitToAttack.Get());
		}

		if (UnitToAttack.IsValid())
		{
			UnitToAttack->AttackTarget(Cast<IRPGAttackable>(GetNearestTarget(MeleeBox)), Cast<IRPGAttackable>(GetNearestTarget(RangedSphere)));
		}
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

void ARPGPlayer::OnUnitRecoveryStateChanged(TWeakObjectPtr<ARPGPlayerUnit> Unit, bool IsInRecovery)
{
	if (IsInRecovery)
	{
		//Set next active unit if available
		if (SelectedUnit == Unit)
		{
			ARPGPlayerUnit* NextAvailableUnit = nullptr;

			for (size_t i = 1; i < Units.Num(); i++)
			{
				int index = Unit->UnitIndex + i;
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
			SetSelectedUnit(Unit.Get());
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
	URPG_EventManager::GetInstance()->SelectedUnitChanged.Broadcast(Unit);
}