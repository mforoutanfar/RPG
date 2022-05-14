// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCreature.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "RPGFunctionLibrary.h"
#include "RPGRandomAudioComponent.h"

#include "RPG_EventManager.h"
#include "RPGPlayerUnit.h"
#include "RPGInventory.h"

// Sets default values
ARPGCreature::ARPGCreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->DestroyComponent();
	GetMesh()->SetActive(false);

	MeleeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeSphere"));
	MeleeSphere->SetupAttachment(RootComponent);

	RangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
	RangeSphere->SetupAttachment(RootComponent);

	HitBox = GetCapsuleComponent();

	AudioComponent = CreateDefaultSubobject<URPGRandomAudioComponent>(FName("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	Inventory = CreateDefaultSubobject<URPGInventory>(FName("Inventory"));
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

FRPGAttackResults ARPGCreature::OnAttacked(FRPGAttackData AttackData)
{
	FRPGAttackResults Results;
	Results.Target = this;

	float DamageDealt = 0.0f;
	DamageDealt += AttackData.PhysicalDamage;
	HP -= DamageDealt;

	Results.DamageDealt = DamageDealt;
	
	AudioComponent->PlayRandom("hit");

	float RecoveryDuration = 1.0f;

	if (HP <= 0)
	{
		Die();
		Results.TargetDied = true;
		RecoveryDuration = -1.0f;
	}

	EnterRecovery(RecoveryDuration);

	return Results;
}

void ARPGCreature::Attack()
{	
	FRPGAttackData AttackData;
	FRPGAttackResults Results;
	float RecoveryDuration = 2.0f;

	if (auto NearestMelee = GetNearestAttackTarget(MeleeSphere))
	{
		AttackData.Attacker = this;
		AttackData.Target = NearestMelee;
		AttackData.PhysicalDamage = MeleeDamage;

		auto Attackable = Cast<IRPGAttackable>(NearestMelee);
		Results = Attackable->OnAttacked(AttackData);
	}
	else if (auto NearestRanged = Cast<IRPGAttackable>(GetNearestAttackTarget(RangeSphere)))
	{

	}
	else
	{

	}

	AudioComponent->PlayRandom("whoosh");
	EnterRecovery(RecoveryDuration);
	URPG_EventManager::GetInstance()->AttackOccured.Broadcast(this, AttackData.Target, Results);
}

void ARPGCreature::Die()
{

}

AActor* ARPGCreature::GetNearestAttackTarget(UShapeComponent* Collider, bool ExcludeOwnType)
{
	AActor* ClosestAttackableActor = nullptr;
	float MinDistance = FLT_MAX;
	TSet<AActor*> OverlappingActors = {};
	Collider->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (auto Creature = Cast<ARPGCreature>(Actor))
		{
			if (ExcludeOwnType && Creature->CreatureType == CreatureType)
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

void ARPGCreature::EnterRecovery(float Duration)
{
	GetWorldTimerManager().ClearTimer(RecoveryTimerHandle);

	InRecovery = true;

	if (Duration >= 0)
	{
		GetWorldTimerManager().SetTimer(RecoveryTimerHandle, this, &ARPGCreature::ExitRecovery, Duration, false);
	}

	URPG_EventManager::GetInstance()->RecoveryStateChanged.Broadcast(this, true);
}

void ARPGCreature::ExitRecovery()
{
	InRecovery = false;
	URPG_EventManager::GetInstance()->RecoveryStateChanged.Broadcast(this, false);
}