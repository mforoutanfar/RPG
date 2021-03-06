// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFunctionLibrary.h"
#include "RPGAttackable.h"
#include "RPGCreature.h"

#include "RPGPlayerUnit.generated.h"

/**
 *
*/
UCLASS()
class RPG_API ARPGPlayerUnit : public ARPGCreature
{
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	*/
	ARPGPlayerUnit();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	void InteractWithTarget(AActor* Target);

	int UnitIndex = -1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FString ClassName = "";

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FString AbilitiesDescription = "";

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int HiringFee = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FColor AvatarColor;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
		void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHitboxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnAddItemToInventoryProposed(AActor* InvOwner, FRPGItemInfo ItemInfo, int ProposedRow, int ProposedCol);

	UFUNCTION()
	void OnAddItemToEquipmentProposed(ARPGCreature* Creature, FRPGItemInfo ItemInfo);


	int EnemiedInMeleeRange = 0;
	int EnemiedInRangedRange = 0;

	UnitSafety::SafetyState CurrentSafetyState = UnitSafety::SafetyState::SAFE;
	void UpdateSafetyState();
private:

};
