// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCreature.h"

#include "RPGBillboardVisuals.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework\CharacterMovementComponent.h"

#include "RPGFunctionLibrary.h"
#include "RPGRandomAudioComponent.h"

// Sets default values
ARPGCreature::ARPGCreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->DestroyComponent();
	GetMesh()->SetActive(false);

	Visuals = CreateDefaultSubobject<URPGBillboardVisuals>(FName("Visuals"));	
	Visuals->SetupAttachment(RootComponent);
	Visuals->Init(CreatureName.ToString());

	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyHitBox"));

	AudioComponent = CreateDefaultSubobject<URPGRandomAudioComponent>(FName("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);	

	//MovementModeChangedDelegate.AddDynamic(this, &ARPGCreature::OnMovementModeChange);
}

void ARPGCreature::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	EMovementMode NewMode = Cast<UCharacterMovementComponent>(GetMovementComponent())->MovementMode;
	switch (NewMode)
	{
	case MOVE_None:
	{
		Visuals->SetAnimState(IDLE);
		break;
	}
	case MOVE_Walking:
	case MOVE_NavWalking:
	{
		Visuals->SetAnimState(WALK);
		break;
	}	
	default:
		break;
	}
}

// Called when the game starts or when spawned
void ARPGCreature::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ARPGCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARPGCreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FRPGAttackResults ARPGCreature::OnAttacked(FRPGAttackData AttackData)
{
	FRPGAttackResults Results;

	if (auto Attacker = AttackData.Attacker.Get())
	{
		auto a = Attacker->GetActorLocation();
		auto DistVec = Attacker->GetActorLocation() - GetActorLocation();
		SetActorRotation(DistVec.Rotation());
	}

	HP -= AttackData.PhysicalDamage;
	AudioComponent->PlayRandom("hit");

	if (HP <= 0)
	{
		Die();
		Results.TargetDied = true;
	}
	else
	{
		Visuals->OnOwnerAttacked();
	}

	return Results;
}

InteractableCat ARPGCreature::GetInteractableType()
{
	if (Dead)
	{
		return InteractableCat::CORPSE;
	}

	return InteractableCat::NONE;
}

void ARPGCreature::OnInteracted(bool Successful)
{
	if (Dead)
	{
		Destroy();
	}
}

void ARPGCreature::Die()
{
	Dead = true;
	GetCapsuleComponent()->SetCollisionProfileName(FName("Interactable"));

	Visuals->OnOwnerDied();
}


