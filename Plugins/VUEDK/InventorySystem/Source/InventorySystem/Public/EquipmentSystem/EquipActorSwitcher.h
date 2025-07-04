// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "Components/ActorComponent.h"
#include "EquipActorSwitcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnActorHold,
	AActor*, Actor
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnActorWithdraw,
	AActor*, Actor
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnActorDisposed,
	AActor*, Actor
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnEquipActorsChanged
);

/**
 * @brief Useful class for spawning and attaching actors to the player based on the current equipment.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UEquipActorSwitcher : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnActorHold OnHoldActorEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnActorWithdraw OnWithdrawActorEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnActorDisposed OnActorDisposedEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnEquipActorsChanged OnEquipActorsChangedEvent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UEquipSlotKey* TargetEquipSlotKey;

private:
	UPROPERTY()
	UEquipment* Equipment;
	UPROPERTY()
	USceneComponent* AttachToComponentEquipped;
	UPROPERTY()
	USceneComponent* AttachToComponentUnequipped;
	FName EquippedAttachSocketName;
	FName UnequippedAttachSocketName;
	UPROPERTY()
	AActor* HeldActor;
	UPROPERTY()
	TMap<int32, AActor*> CurrentActors;

public:
	UEquipActorSwitcher();
	
	UFUNCTION(BlueprintCallable)
	void Init(UEquipment* InEquipment, USceneComponent* InAttachToComponentEquipped, FName InEquipppedAttachSocketName, USceneComponent* InAttachToComponentUnequipped, FName InUnequipppedAttachSocketName);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintPure)
	AActor* GetHeldActor() const;

	UFUNCTION(BlueprintPure)
	bool IsHoldingActor() const;

	UFUNCTION(BlueprintCallable)
	void HoldNextActor();

	UFUNCTION(BlueprintCallable)
	void HoldPreviousActor();

	UFUNCTION(BlueprintCallable)
	void HoldFirstActor();

	UFUNCTION(BlueprintCallable)
	void HoldLastActor();

	UFUNCTION(BlueprintCallable)
	void HoldActorAtSlot(const int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void WithdrawActorAtSlot(const int32 SlotIndex) const;

	UFUNCTION(BlueprintPure)
	TMap<int32, AActor*> GetCurrentActors() const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnHoldActor(AActor* Actor) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnWithdrawActor(AActor* Actor) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnActorDisposed(AActor* Actor) const;

	UFUNCTION(BlueprintNativeEvent)
	AActor* CreateActorToHold(UItemBase* Item) const;

	UFUNCTION(BlueprintNativeEvent)
	void DisposeActor(AActor* Actor);

	UFUNCTION(BlueprintNativeEvent)
	bool CanAddActor(const UItemBase* Item) const;

	void AddEquipActor(UItemBase* Item, const int32 SlotIndex);

	void RemoveEquipActor(const int32 SlotIndex, bool FindNewActorToHold = true);

	int32 FindNextAvailableSlotIndex() const;

	int32 FindPreviousAvailableSlotIndex() const;

	int32 GetMaxSlotIndex() const;

	int32 GetMinSlotIndex() const;

	int32 GetHeldActorSlotIndex() const;

private:
	void HoldActor(AActor* Actor);

	void WithdrawActor(AActor* Actor) const;

	void ChangeEquipActorSlot(const int32 OldSlotIndex, const int32 NewSlotIndex);

	void SwitchEquipActorsSlots(const int OldSlotIndexItemA, const int NewSlotIndexItemA, const int OldSlotIndexItemB, const int NewSlotIndexItemB);

	bool IsTargetEquipSlotKeyValid(const UEquipSlotKey* EquipSlotKey) const;
	
	bool Check() const;

	UFUNCTION()
	void OnAnyItemEquipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* Item);

	UFUNCTION()
	void OnAnyItemUnequipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* Item);

	UFUNCTION()
	void OnAnyItemEquipSlotChanged(UEquipSlotKey* EquipSlotKey, UItemBase* Item, int32 NewSlotIndex, int32 OldSlotIndex);

	UFUNCTION()
	void OnItemsSwitchedSlots(UEquipSlotKey* EquipSlotKey, UItemBase* ItemA, UItemBase* ItemB, int OldSlotIndexItemA, int OldSlotIndexItemB, int NewSlotIndexItemA, int NewSlotIndexItemB);

	UFUNCTION()
	void OnAnyItemSwapWithInventoryItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item, UItemBase* InventoryItem, int32 SlotIndex);
};
