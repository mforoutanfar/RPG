// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCreature.h"

#include "RPGBillboardVisuals.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework\CharacterMovementComponent.h"

#include "RPGFunctionLibrary.h"
#include "RPGRandomAudioComponent.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("PerceptionComponent"));	

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("SightConfig"));

	SightConfig->SightRadius = 7000.0f;
	SightConfig->LoseSightRadius = 8000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 142.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->SetMaxAge(45.0f);

	PerceptionComponent->ConfigureSense(*SightConfig);

	PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

	//MovementModeChangedDelegate.AddDynamic(this, &ARPGCreature::OnMovementModeChange);
}

// Called when the game starts or when spawned
void ARPGCreature::BeginPlay()
{
	Super::BeginPlay();	

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ARPGCreature::OnTargetPerceptionUpdated);
}

void ARPGCreature::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	auto Blackboard = GetController()->FindComponentByClass<UBlackboardComponent>();
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(FName("PlayerInSight"), Stimulus.WasSuccessfullySensed());
	}
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
	Results.Target = this;

	if (auto Attacker = AttackData.Attacker.Get())
	{
		auto a = Attacker->GetActorLocation();
		auto DistVec = Attacker->GetActorLocation() - GetActorLocation();
		SetActorRotation(DistVec.Rotation());
	}

	float DamageDealt = 0.0f;
	DamageDealt += AttackData.PhysicalDamage;
	HP -= DamageDealt;

	Results.DamageDealt = DamageDealt;

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


