// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayer.h"

#include "GameFramework\CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "RPG\RPGInteractable.h"
#include "RPG\RPGPlayerUnit.h"

// Sets default values
ARPGPlayer::ARPGPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCameraComponent->bUsePawnControlRotation = true;	
	PlayerCameraComponent->bLockToHmd = false;

	InteractionCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionCollider"));
	InteractionCollider->SetCollisionProfileName(FName("Interactor"));

	MeleeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeBox"));
	MeleeBox->SetCollisionProfileName(FName("PlayerDamageSource"));

	RangedSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangedSphere"));
	RangedSphere->SetCollisionProfileName(FName("PlayerDamageSource"));

	float UnitOffset = 100.0f;
	int NumberOfUnits = 4;

	for (size_t i = 0; i < NumberOfUnits; i++)
	{
		auto Name = FString::Printf(TEXT("Unit %d"), i);
		auto Unit = CreateDefaultSubobject<UChildActorComponent>(FName(*Name));
		auto posY = -(NumberOfUnits - 1) * UnitOffset / 2.0f + i * UnitOffset;
		Unit->SetRelativeLocation(FVector(0.0f, posY, 0.0f));
		Units.Add(Unit);
	}

	CurrentUnit = Units[0];

}

void ARPGPlayer::OnConstruction(const FTransform& Transform)
{
	PlayerCameraComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	InteractionCollider->AttachToComponent(PlayerCameraComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MeleeBox->AttachToComponent(PlayerCameraComponent, FAttachmentTransformRules::KeepRelativeTransform);
	RangedSphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	for (auto Unit : Units)
	{
		Unit->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		auto ChildActor = Cast<UChildActorComponent>(Unit);
		ChildActor->SetChildActorClass(UnitClass);		
	}
}

// Called when the game starts or when spawned
void ARPGPlayer::BeginPlay()
{
	Super::BeginPlay();	
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
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Released, this, &ARPGPlayer::OnInteractReleased);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Released, this, &ARPGPlayer::OnAttackReleased);
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

void ARPGPlayer::OnInteractReleased()
{
	//Find nearest interactable object
	AActor* ClosestInteractableActor = nullptr;
	float MinDistance = FLT_MAX;
	TSet<AActor*> OverlappingActors = {};
	InteractionCollider->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor :  OverlappingActors)
	{
		if (Actor->GetClass()->ImplementsInterface(URPGInteractable::StaticClass()))
		{
			auto Distance = (Actor->GetActorLocation() - GetActorLocation()).SizeSquared();
			if (Distance < MinDistance)
			{
				ClosestInteractableActor = Actor;
				MinDistance = Distance;
			}
		}
	}
	
	if (ClosestInteractableActor)
	{
		Cast<IRPGInteractable>(ClosestInteractableActor)->OnInteracted();
	}	
}

void ARPGPlayer::OnAttackReleased()
{
	//Find nearest attackable object
	AActor* ClosestAttackableActor = nullptr;
	float MinDistance = FLT_MAX;
	TSet<AActor*> OverlappingActors = {};
	MeleeBox->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		auto Distance = (Actor->GetActorLocation() - GetActorLocation()).SizeSquared();
		if (Distance < MinDistance)
		{
			ClosestAttackableActor = Actor;
			MinDistance = Distance;
		}
	}

	if (ClosestAttackableActor)
	{
		auto curUnit = Cast<ARPGPlayerUnit>(CurrentUnit->GetChildActor());
		if (curUnit->MeleeDamage > 0)
		{
			ClosestAttackableActor->Destroy();
		}
	}
}