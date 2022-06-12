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

	//For AI Sight Detection. TODO: Better alternative?
	Tags.Add(FName("Player"));

	DamagePerFallDuration.k1 = 5;
	DamagePerFallDuration.Dn = 2;
	DamagePerFallDuration.k2 = 20;
	//SightSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("SightSource"));
	//SightSource->RegisterForSense(UAISense_Sight::StaticClass());	
}

void ARPGPlayer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	PlayerCameraComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	InteractionCollider->AttachToComponent(PlayerCameraComponent, FAttachmentTransformRules::KeepRelativeTransform);	

	InteractionCollider->OnComponentBeginOverlap.AddDynamic(this, &ARPGPlayer::OnInteractionColliderBeginOverlap);
	InteractionCollider->OnComponentEndOverlap.AddDynamic(this, &ARPGPlayer::OnInteractionColliderEndOverlap);
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
	RPGEventManager->AvatarClicked.AddDynamic(this, &ARPGPlayer::OnUnitAvatarClicked);
	RPGEventManager->CreatureDied.AddDynamic(this, &ARPGPlayer::OnCreatureDied);
	OnReachedJumpApex.AddDynamic(this, &ARPGPlayer::OnOnReachedJumpApex);

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

	TSet<AActor*> OverlappingActors;
	InteractionCollider->GetOverlappingActors(OverlappingActors);
	int NumOfInteractables = 0;
	for (auto i : OverlappingActors)
	{
		if (auto Interactable = Cast<IRPGInteractable>(i))
		{
			NumOfInteractables++;
		}
	}
	InteractablesInRange = NumOfInteractables;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ARPGPlayer::OnOnReachedJumpApex()
{
	GetWorld()->GetTimerManager().SetTimer(FallingHandle, 600.0f, false);
}

// Called every frame
void ARPGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InteractablesInRange > 0)
	{
		auto Nearest = GetNearestTarget(InteractionCollider, true, true);

		if (Nearest != NearestInteractable)
		{
			NearestInteractable = Nearest;

			RPGEventManager->NearestInteractableChanged.Broadcast(NearestInteractable);
		}
	}
	else if (NearestInteractable)
	{
		NearestInteractable = nullptr;
		RPGEventManager->NearestInteractableChanged.Broadcast(NearestInteractable);
	}

	UpdateWalkingSoundState();	
}

//TODO: Implement State Machine?
void ARPGPlayer::UpdateWalkingSoundState()
{
	auto mc = GetCharacterMovement();

	bool IsOnGround = !mc->IsFalling();

	float VelocitySq = mc->Velocity.SizeSquared();

	bool IsWalking = !FMath::IsNearlyZero(VelocitySq);

	bool IsRunning = FMath::IsNearlyEqual(VelocitySq, (RunCoeff * WalkSpeed)* (RunCoeff * WalkSpeed), WalkSpeed* WalkSpeed);

	if (OnGround != IsOnGround)
	{
		if (IsOnGround)
		{
			float FallDuration = GetWorld()->GetTimerManager().GetTimerElapsed(FallingHandle);

			if (FallDuration > SafeFallDuration)
			{
				for (auto i : Units)
				{
					float FallDamage = DamagePerFallDuration.GetResult() * FallDuration;

					FRPGAttackData AttackData;
					FRPGAttackResults Results;
					float RecoveryDuration = 1.0f;

					AttackData.Attacker = nullptr;
					AttackData.Target = i;
					AttackData.Accuracy = 1.0f;

					AttackData.Damage = FallDamage;

					auto Attackable = Cast<IRPGAttackable>(i);
					Attackable->OnAttacked(AttackData, Results);

					Results.RecoveryDuration = RecoveryDuration;

					RPGEventManager->AttackOccured.Broadcast(nullptr, AttackData, Results);
				}
			}
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(FallingHandle, 600.0f, false);
		}
	}

	if (OnGround != IsOnGround || Walking != IsWalking || Running != IsRunning)
	{
		if (IsOnGround)
		{
			if (IsWalking)
			{
				if (!IsRunning)
				{
					AudioComponent->SetWalkingSoundPlaying(true, 0.9f);
				}
				else
				{
					AudioComponent->SetWalkingSoundPlaying(true, 0.3f);
				}
			}
			else
			{
				AudioComponent->SetWalkingSoundPlaying(false, 1.0f);
			}
		}
		else
		{
			AudioComponent->SetWalkingSoundPlaying(false, 1.0f);
		}
	}

	OnGround = IsOnGround;
	Walking = IsWalking;
	Running = IsRunning;

	FString st = "";
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
	GetCharacterMovement()->bNotifyApex = true;
	Jump();
}

void ARPGPlayer::OnJumpReleased()
{
	StopJumping();
}

void ARPGPlayer::OnRunPressed()
{
	GetCharacterMovement()->MaxWalkSpeed *= RunCoeff;
}

void ARPGPlayer::OnRunReleased()
{
	GetCharacterMovement()->MaxWalkSpeed /= RunCoeff;
}

void ARPGPlayer::OnInteractPressed()
{
	ARPGPlayerUnit* UnitToInteract = nullptr;

	if (NearestInteractable)
	{
		if (SelectedUnit.IsValid())
		{
			if (!SelectedUnit.Get()->IsInRecovery())
			{
				UnitToInteract = SelectedUnit.Get();
			}
		}
		else
		{
			UnitToInteract = FindFirstOutOfRecoveryUnit();
		}

		auto Interactable = Cast<IRPGInteractable>(NearestInteractable);
		auto Type = Interactable->GetInteractableType();

		if (Type == ITEM || Type == CORPSE || Type == BUTTON)
		{
			if (!UnitToInteract)
			{
				UnitToInteract = Units[0];
			}

			UnitToInteract->InteractWithTarget(NearestInteractable);
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
		UnitToAttack->BeginAttack();
	}
}

void ARPGPlayer::OnSpellPressed()
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
		UnitToAttack->CastReadySpell();
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

void ARPGPlayer::OnUnitAvatarClicked(ARPGPlayerUnit* Unit, FName ButtonName)
{
	if (ButtonName == "LeftMouseButton")
	{
		SetSelectedUnit(Unit);
	}
	else if (ButtonName == "RightMouseButton")
	{

	}
}

void ARPGPlayer::OnInteractionColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Interactable = Cast<IRPGInteractable>(OtherActor))
	{
		InteractablesInRange++;
	}
}

//TODO: Not very optimal. (Called whenever enemies die too)
void ARPGPlayer::OnCreatureDied(ARPGCreature* Unit)
{
	for (auto i: Units)
	{
		if (!i->IsDead())
		{
			return;
		}
	}

	RPGEventManager->GameOverIssued.Broadcast();
}

void ARPGPlayer::OnInteractionColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (auto Interactable = Cast<IRPGInteractable>(OtherActor))
	{
		InteractablesInRange--;
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

AActor* ARPGPlayer::GetNearestTarget(UShapeComponent* Collider, bool ShouldBeVisible, bool ShouldBeInteractable)
{
	AActor* NearestTarget = nullptr;
	float MinDistance = FLT_MAX;
	TSet<AActor*> OverlappingActors = {};
	Collider->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (ShouldBeVisible)
		{
			FVector2D pos;
			if (!UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(this, 0), Actor->GetActorLocation(), pos))
			{
				continue;
			}
		}

		if (ShouldBeInteractable)
		{
			auto Interactable = Cast<IRPGInteractable>(Actor);
			if (!Interactable)
			{
				continue;
			}
			else if (!Interactable->IsInteractable())
			{
				continue;
			}
		}

		auto Distance = (Actor->GetActorLocation() - GetActorLocation()).SizeSquared();
		if (Distance < MinDistance)
		{
			NearestTarget = Actor;
			MinDistance = Distance;
		}
	}

	return NearestTarget;
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