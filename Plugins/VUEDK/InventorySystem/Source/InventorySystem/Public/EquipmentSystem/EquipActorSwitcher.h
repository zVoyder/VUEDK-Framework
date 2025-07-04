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
	/**
	 * Default constructor for UEquipActorSwitcher.
	 */
	UEquipActorSwitcher();
	
	/**
	 * Initializes the EquipActorSwitcher with the given equipment and attach components.
	 * @param InEquipment The equipment to associate with this switcher.
	 * @param InAttachToComponentEquipped The component to attach to when equipped.
	 * @param InEquipppedAttachSocketName The socket name to use when equipped.
	 * @param InAttachToComponentUnequipped The component to attach to when unequipped.
	 * @param InUnequipppedAttachSocketName The socket name to use when unequipped.
	 */
	UFUNCTION(BlueprintCallable)
	void Init(UEquipment* InEquipment, USceneComponent* InAttachToComponentEquipped, FName InEquipppedAttachSocketName, USceneComponent* InAttachToComponentUnequipped, FName InUnequipppedAttachSocketName);

	/**
	 * Called when the actor is being removed from the world.
	 * @param EndPlayReason The reason why the actor is ending play.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Returns the currently held actor.
	 * @return Pointer to the held AActor.
	 */
	UFUNCTION(BlueprintPure)
	AActor* GetHeldActor() const;

	/**
	 * Checks if an actor is currently being held.
	 * @return True if holding an actor, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsHoldingActor() const;

	/**
	 * Switches to hold the next actor in the sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void HoldNextActor();

	/**
	 * Switches to hold the previous actor in the sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void HoldPreviousActor();

	/**
	 * Switches to hold the first actor in the sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void HoldFirstActor();

	/**
	 * Switches to hold the last actor in the sequence.
	 */
	UFUNCTION(BlueprintCallable)
	void HoldLastActor();

	/**
	 * Switches to hold the actor at the specified slot index.
	 * @param SlotIndex The index of the slot to hold.
	 */
	UFUNCTION(BlueprintCallable)
	void HoldActorAtSlot(const int32 SlotIndex);

	/**
	 * Withdraws the actor at the specified slot index.
	 * @param SlotIndex The index of the slot to withdraw.
	 */
	UFUNCTION(BlueprintCallable)
	void WithdrawActorAtSlot(const int32 SlotIndex) const;

	/**
	 * Returns the current map of slot indices to actors.
	 * @return Map of slot indices to AActor pointers.
	 */
	UFUNCTION(BlueprintPure)
	TMap<int32, AActor*> GetCurrentActors() const;

protected:
	/**
	 * Called when an actor is held.
	 * @param Actor The actor that is now held.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnHoldActor(AActor* Actor) const;

	/**
	 * Called when an actor is withdrawn.
	 * @param Actor The actor that is now withdrawn.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWithdrawActor(AActor* Actor) const;

	/**
	 * Called when an actor is disposed.
	 * @param Actor The actor that is now disposed.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnActorDisposed(AActor* Actor) const;

	/**
	 * Creates an actor to hold for the given item.
	 * @param Item The item to create an actor for.
	 * @return Pointer to the created AActor.
	 */
	UFUNCTION(BlueprintNativeEvent)
	AActor* CreateActorToHold(UItemBase* Item) const;

	/**
	 * Disposes the given actor.
	 * @param Actor The actor to dispose.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void DisposeActor(AActor* Actor);

	/**
	 * Checks if an actor can be added for the given item.
	 * @param Item The item to check.
	 * @return True if the actor can be added, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool CanAddActor(const UItemBase* Item) const;

	/**
	 * Adds an equip actor for the specified item at the given slot index.
	 * @param Item The item for which to add the actor.
	 * @param SlotIndex The slot index where the actor should be added.
	 */
	void AddEquipActor(UItemBase* Item, const int32 SlotIndex);

	/**
	 * Removes the equip actor at the specified slot index.
	 * @param SlotIndex The slot index of the actor to remove.
	 * @param FindNewActorToHold If true, finds a new actor to hold after removal (default: true).
	 */
	void RemoveEquipActor(const int32 SlotIndex, bool FindNewActorToHold = true);

	/**
	 * Finds the next available slot index for holding an actor.
	 * @return The next available slot index, or -1 if none found.
	 */
	int32 FindNextAvailableSlotIndex() const;

	/**
	 * Finds the previous available slot index for holding an actor.
	 * @return The previous available slot index, or -1 if none found.
	 */
	int32 FindPreviousAvailableSlotIndex() const;

	/**
	 * Returns the maximum slot index currently available.
	 * @return The maximum slot index.
	 */
	int32 GetMaxSlotIndex() const;

	/**
	 * Returns the minimum slot index currently available.
	 * @return The minimum slot index.
	 */
	int32 GetMinSlotIndex() const;

	/**
	 * Returns the slot index of the currently held actor.
	 * @return The slot index of the held actor, or -1 if none is held.
	 */
	int32 GetHeldActorSlotIndex() const;

private:
	/**
	 * Internally holds the specified actor.
	 * @param Actor The actor to hold.
	 */
	void HoldActor(AActor* Actor);

	/**
	 * Internally withdraws the specified actor.
	 * @param Actor The actor to withdraw.
	 */
	void WithdrawActor(AActor* Actor) const;

	/**
	 * Changes the equip actor from one slot index to another.
	 * @param OldSlotIndex The old slot index.
	 * @param NewSlotIndex The new slot index.
	 */
	void ChangeEquipActorSlot(const int32 OldSlotIndex, const int32 NewSlotIndex);

	/**
	 * Switches the equip actors between two slot indices.
	 * @param OldSlotIndexItemA The old slot index for item A.
	 * @param NewSlotIndexItemA The new slot index for item A.
	 * @param OldSlotIndexItemB The old slot index for item B.
	 * @param NewSlotIndexItemB The new slot index for item B.
	 */
	void SwitchEquipActorsSlots(const int OldSlotIndexItemA, const int NewSlotIndexItemA, const int OldSlotIndexItemB, const int NewSlotIndexItemB);

	/**
	 * Checks if the provided equip slot key is valid for the target.
	 * @param EquipSlotKey The equip slot key to check.
	 * @return True if valid, false otherwise.
	 */
	bool IsTargetEquipSlotKeyValid(const UEquipSlotKey* EquipSlotKey) const;
	
	/**
	 * Checks the validity or state of the EquipActorSwitcher.
	 * @return True if valid, false otherwise.
	 */
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
