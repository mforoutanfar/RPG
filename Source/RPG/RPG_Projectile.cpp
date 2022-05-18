// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Projectile.h"
#include "RPGAttackable.h"
#include "RPG_EventManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "RPG_GameStateBase.h"

/**
 * Sets default values
*/
ARPG_Projectile::ARPG_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARPG_Projectile::OnHit);
	SphereComponent->SetCollisionProfileName(FName("PlayerDamageSource"));

	MovementComponent->InitialSpeed = 4000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void ARPG_Projectile::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Attackable = Cast<IRPGAttackable>(OtherActor))
	{
		FRPGAttackData Data;
		Data.Damage = 30.0f;		
		Data.Target = OtherActor;
		auto Results = Attackable->OnAttacked(Data);

		RPGEventManager->AttackOccured.Broadcast(this, Data.Target, Results);
	}
	Destroy();
}


/**
 * Called when the game starts or when spawned
*/
void ARPG_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

/**
 * Called every frame
 * @param DeltaTime Game time elapsed during last frame modified by the time dilation
*/
void ARPG_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

