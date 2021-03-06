// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_AvatarWidget.h"
#include "RPGPlayerUnit.h"
#include "RPGPlayer.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "RPG_EventManager.h"
#include "RPG_GameStateBase.h"
#include "RPGPlayer.h"
#include "RPG_DamageNumberWidget.h"
#include "RPG_HUD.h"
#include "RPG_GameHUD.h"
#include "RPG_ItemWidget.h"
#include "RPGInteractable.h"
#include "RPG_Container.h"

void URPG_AvatarWidget::Init(AActor* Unit)
{
	ReferencedUnit = Unit;
	AddListenersOnce();
	//Put here because when OnSafetyStateChanged is called on PlayerUnit's BeginPlay, AvatarWidget is not created yet and doesn't hear it. TODO: Solution?

	if (auto Player = Cast<ARPGPlayerUnit>(Unit))
	{
		DefaultSafetyColor = SafeColor;
		ResetToDefaultColor();
		Portrait->SetColorAndOpacity(Player->AvatarColor);

		if (FMath::IsNearlyZero(Player->MaxMana))
		{
			ManaGroup->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else//Assuming it's container.
	{
		SelectedIndicator->SetBrushTintColor(SelectedColor);
	}
}

void URPG_AvatarWidget::AddListenersOnce()
{
	if (!HasAddedListeners)
	{
		RPGEventManager->RecoveryStateChanged.AddDynamic(this, &URPG_AvatarWidget::OnRecoveryStateChanged);
		RPGEventManager->AttackOccured.AddDynamic(this, &URPG_AvatarWidget::OnAttackOccured);
		RPGEventManager->InteractionOccured.AddDynamic(this, &URPG_AvatarWidget::OnInteractionOccured);
		RPGEventManager->SelectedUnitChanged.AddDynamic(this, &URPG_AvatarWidget::OnSelectedUnitChanged);
		RPGEventManager->ContainerFocusStateChanged.AddDynamic(this, &URPG_AvatarWidget::OnContainerFocusStateChanged);
		RPGEventManager->InventoryItemAdded.AddDynamic(this, &URPG_AvatarWidget::OnInventoryItemAdded);
		RPGEventManager->SafetyStateChanged.AddDynamic(this, &URPG_AvatarWidget::OnSafetyStateChanged);
		RPGEventManager->CreatureStateChanged.AddDynamic(this, &URPG_AvatarWidget::OnCreatureStateChanged);
		RPGEventManager->SpellCast.AddDynamic(this, &URPG_AvatarWidget::OnSpellCast);
		HasAddedListeners = true;
	}
}

void URPG_AvatarWidget::OnCreatureStateChanged(ARPGCreature* Creature)
{	 
	if (Creature == ReferencedUnit.Get())
	{
		auto HPPercent = Creature->HP / Creature->MaxHP;
		HPBar->SetPercent(HPPercent);

		auto ManaPercent = Creature->Mana / Creature->MaxMana;
		ManaBar->SetPercent(ManaPercent);
	}
}

void URPG_AvatarWidget::OnSpellCast(ARPGCreature* Creature)
{
	if (Creature == ReferencedUnit.Get())
	{
		Portrait->SetBrushFromTexture(AvatarMap[SPELL]);

		GetWorld()->GetTimerManager().ClearTimer(ResetAvatarHandle);
		GetWorld()->GetTimerManager().SetTimer(ResetAvatarHandle, this, &URPG_AvatarWidget::ResetAvatar, ResetDelay, true);
	}
}

void URPG_AvatarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RecoveryIndicator->SetBrushTintColor(SafeColor);
	SelectedIndicator->SetBrushTintColor(DeselectedColor);	
}

URPG_AvatarWidget::URPG_AvatarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FString Codes[9] = {"normal", "angry", "sad", "dead", "disappointed", "happy", "pain", "cocky", "spell"};
		
	for (size_t i = 0; i < 9; i++)
	{
		FString AssetName = "avatar_dummy_" + Codes[i];
		FString PathToLoad = FString("/Game/Assets/Avatars/") + AssetName + FString(".") + AssetName;

		UTexture2D* tmpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(PathToLoad)));
		AvatarMap.Add(AvatarState(i), tmpTexture);
	}
}

void URPG_AvatarWidget::OnSafetyStateChanged(ARPGPlayerUnit* Unit, TEnumAsByte<UnitSafety::SafetyState> State)
{
	if (ReferencedUnit.Get() != Unit)
	{
		return;
	}

	switch (State)
	{
	case UnitSafety::SAFE:
	{
		DefaultSafetyColor = SafeColor;
		break;
	}	
	case UnitSafety::WARNING:
	{
		DefaultSafetyColor = WarningColor;
		break;
	}
	case UnitSafety::DANGER:
	{
		DefaultSafetyColor = DangerColor;
		break;
	}
	default:
		break;
	}

	//TODO: Multiple definitions of InRecovery (Both here and in Creature). Solution?
	if (!InRecovery)
	{
		ResetToDefaultColor();
	}	
}

void URPG_AvatarWidget::OnRecoveryStateChanged(AActor* Unit, bool State)
{
	if (ReferencedUnit.Get() != Unit)
	{
		return;
	}

	InRecovery = State;

	if (State)
	{
		RecoveryIndicator->SetBrushTintColor(RecoveryColor);
	}
	else
	{
		ResetToDefaultColor();
	}
}

void URPG_AvatarWidget::ResetToDefaultColor()
{
	RecoveryIndicator->SetBrushTintColor(DefaultSafetyColor);
}

//TODO: All avatar changes can be made using UMG animations in editor. This way they won't pause when the game is paused.
void URPG_AvatarWidget::OnInventoryItemAdded(URPGInventoryItem* Item, AActor* Owner)
{
	if (auto Unit = Cast<ARPGPlayerUnit>(Owner))
	{
		if (Unit == ReferencedUnit.Get())
		{
			Portrait->SetBrushFromTexture(AvatarMap[HAPPY]);

			GetWorld()->GetTimerManager().ClearTimer(ResetAvatarHandle);
			GetWorld()->GetTimerManager().SetTimer(ResetAvatarHandle, this, &URPG_AvatarWidget::ResetAvatar, ResetDelay, true);			
		}
	}
}

void URPG_AvatarWidget::OnInteractionOccured(AActor* Interactor, TEnumAsByte<InteractableCategory::InteractableCat> Category, bool Successful)
{
	if (Interactor == ReferencedUnit.Get())
	{
		AvatarState Reaction = NORMAL;

		if (Successful)
		{
			switch (Category)
			{
			case InteractableCategory::ITEM:
			case InteractableCategory::CHEST:
			case InteractableCategory::CORPSE:
			{
				Reaction = HAPPY;
				break;
			}			
			case InteractableCategory::LOCKED_CHEST:
			{
				Reaction = COCKY;
				break;
			}			
			default:
				break;
			}
		}
		else
		{
			switch (Category)
			{
			case InteractableCategory::ITEM:
			case InteractableCategory::DOOR:
			case InteractableCategory::LOCKED_DOOR:
			case InteractableCategory::CHEST:
			case InteractableCategory::LOCKED_CHEST:
			case InteractableCategory::BUTTON:
			{
				Reaction = DISAPPOINTED;
			}
			default:
				break;
			}
		}

		if (Reaction != NORMAL)
		{
			Portrait->SetBrushFromTexture(AvatarMap[Reaction]);

			GetWorld()->GetTimerManager().ClearTimer(ResetAvatarHandle);
			GetWorld()->GetTimerManager().SetTimer(ResetAvatarHandle, this, &URPG_AvatarWidget::ResetAvatar, ResetDelay, true);
		}
	}
}

void URPG_AvatarWidget::OnAttackOccured(AActor* Attacker, FRPGAttackData Data, FRPGAttackResults Results)
{
	//TODO: Shouldn't Access Unit Directly?
	auto Unit = ReferencedUnit.Get();

	if (Unit == Attacker)//Is Attacker
	{
		if (Results.TargetDied)
		{
			Portrait->SetBrushFromTexture(AvatarMap[COCKY]);
		}
		else
		{
			Portrait->SetBrushFromTexture(AvatarMap[ANGRY]);
		}

		GetWorld()->GetTimerManager().ClearTimer(ResetAvatarHandle);
		GetWorld()->GetTimerManager().SetTimer(ResetAvatarHandle, this, &URPG_AvatarWidget::ResetAvatar, ResetDelay, true);
	}
	else if (ReferencedUnit.Get() == Data.Target)//Is Attacked
	{
		auto Creature = Cast<ARPGCreature>(Data.Target);

		//TODO: Unify with OnCreatureStateChanged?
		auto percent = HPBar->Percent - Results.DamageDealt / Creature->MaxHP;
		HPBar->SetPercent(percent);

		GetWorld()->GetTimerManager().ClearTimer(ResetAvatarHandle);
		
		if (Results.Missed)
		{
			Portrait->SetBrushFromTexture(AvatarMap[COCKY]);
			GetWorld()->GetTimerManager().SetTimer(ResetAvatarHandle, this, &URPG_AvatarWidget::ResetAvatar, ResetDelay, true);
		}
		else if (percent > 0)
		{
			Portrait->SetBrushFromTexture(AvatarMap[PAIN]);
			GetWorld()->GetTimerManager().SetTimer(ResetAvatarHandle, this, &URPG_AvatarWidget::ResetAvatar, ResetDelay, true);
		}
		else
		{
			//TODO: Implement OnDie?
			Portrait->SetBrushFromTexture(AvatarMap[DEAD]);
		}
	}
}

void URPG_AvatarWidget::OnSelectedUnitChanged(ARPGPlayerUnit* Unit)
{
	if (ReferencedUnit.Get() == Unit)
	{
		SelectedIndicator->SetBrushTintColor(SelectedColor);
	}
	else
	{
		SelectedIndicator->SetBrushTintColor(DeselectedColor);
	}
}

void URPG_AvatarWidget::OnContainerFocusStateChanged(AActor* Container, bool IsFocused)
{
	if (Container == ReferencedUnit.Get())
	{
		if (IsFocused)
		{
			SelectedIndicator->SetBrushTintColor(SelectedColor);
		}
		else
		{
			SelectedIndicator->SetBrushTintColor(DeselectedColor);
		}
	}
}


void URPG_AvatarWidget::ResetAvatar()
{
	Portrait->SetBrushFromTexture(AvatarMap[NORMAL]);
}

FReply URPG_AvatarWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto Button = InMouseEvent.GetEffectingButton();

	if (Button.GetFName() == "LeftMouseButton")
	{
		OnLeftClick(Button);
	}
	else if (Button.GetFName() == "RightMouseButton")
	{
		OnRightClick();
	}	

	return FReply::Handled();
}

void URPG_AvatarWidget::OnRightClick()
{
	if (RPGGameHUD->PickedItem)
	{
		if (auto Creature = Cast<ARPGCreature>(ReferencedUnit.Get()))
		{
			if (RPGGameHUD->PickedItem->ItemInfo.ItemCategory == ItemCategory::ItemCat::CONSUMABLE)
			{
				RPGEventManager->ConsumeItemProposed.Broadcast(RPGGameHUD->PickedItem->ItemInfo, Creature);
			}
		}
	}
}

void URPG_AvatarWidget::OnLeftClick(FKey& Button)
{
	if (RPGGameHUD->PickedItem)
	{
		RPGEventManager->AddItemToInventoryProposed.Broadcast(ReferencedUnit.Get(), RPGGameHUD->PickedItem->ItemInfo, 0,0);
	}
	else
	{
		RPGEventManager->AvatarClicked.Broadcast(ReferencedUnit.Get(), Button.GetFName());

		if (auto Container = Cast<ARPG_Container>(ReferencedUnit.Get()))
		{
			SelectedIndicator->SetBrushTintColor(SelectedColor);
			RPGEventManager->ContainerFocusStateChanged.Broadcast(Container, true);
		}
	}
}

FReply URPG_AvatarWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}
