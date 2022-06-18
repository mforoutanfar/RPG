// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGFunctionLibrary.h"

#include "RPG_GameHUD.generated.h"

class ARPGPlayerUnit;
class URPG_ItemWidget;
class URPG_AvatarWidget;

/**
 * 
 */
UCLASS()
class RPG_API URPG_GameHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPG_AvatarWidget> AvatarClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URPG_FollowerWidget> SlashWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URPG_DamageNumberWidget> DamageNumberWidgetClass;

	void OnOpenInventoryPressed(bool InventoryOpen);

	void OnPausePressed(bool PauseMenuOpen);

	void RegisterOnMinimap(AActor* Actor, TEnumAsByte<MiniMap::ObjectType> Type);
	void UnregisterFromMinimap(AActor* Actor);

	URPG_ItemWidget* PickedItem = nullptr;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class URPG_ItemWidget> ItemClass;

	UPROPERTY(meta = (BindWidget))
	class URPG_InteractablePing* InteractionPing = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CoinValue = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* HorBox = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MessageText = nullptr;

	void OnStoryEventTriggered(FString EventName);

	void CloseStoryEvent();

	UUserWidget* CurrentStoryEvent = nullptr;

	FString CurrentEventName = "";

	UFUNCTION(BlueprintCallable)
	void ShowMessage(FString Message, float Duration = 5.0f);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnContainerFocusStateChanged(AActor* Container, bool IsFocused);

	FTimerHandle MessageTimerHandle;

	void HideMessage();

	UFUNCTION()
	void OnCoinValueChanged(int Value);

	UFUNCTION()
	void OnRemovePickedItemProposed();

	UFUNCTION()
	void OnItemWidgetPicked(URPG_ItemWidget* ItemWidget);

	UFUNCTION()
	void OnEquipmentItemReplaced(FRPGItemInfo PreviousItemInfo);

	UFUNCTION()
	void OnNearestInteractableChanged(AActor* NearestInteractable);
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* AvatarsBox;

	UPROPERTY(meta = (BindWidget))
	UWidget* InventoryGroup;

	UPROPERTY(meta = (BindWidget))
	UWidget* PauseMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* Equipment;

	UPROPERTY(meta = (BindWidget))
	UWidget* Background;

	UPROPERTY(meta = (BindWidget))
		URPG_AvatarWidget* ContainerAvatar;

	UPROPERTY(meta = (BindWidget))
		UWidget* ContainerGroup;

	UPROPERTY(meta = (BindWidget))
	class URPG_MiniMapWidget* MiniMap;

	UFUNCTION()
	void OnUnitAdded(ARPGPlayerUnit* Unit);

	UFUNCTION()
	void OnAttackOccured(AActor* Attacker, FRPGAttackData Data, FRPGAttackResults Results);

	TMap<ARPGPlayerUnit*, URPG_AvatarWidget*> AvatarMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSubclassOf<UUserWidget>> StoryEventMap;
};
