// Copyright VUEDK, Inc. All Rights Reserved.

#include "EquipmentSystem/EquipActorSwitcher.h"
#include "InventorySystem.h"
#include "Base/Data/ItemDataBase.h"

UEquipActorSwitcher::UEquipActorSwitcher(): TargetEquipSlotKey(nullptr),
                                            Equipment(nullptr),
                                            AttachToComponentEquipped(nullptr),
                                            AttachToComponentUnequipped(nullptr),
                                            HeldActor(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipActorSwitcher::Init(UEquipment* InEquipment, USceneComponent* InAttachToComponentEquipped, FName InEquipppedAttachSocketName, USceneComponent* InAttachToComponentUnequipped, FName InUnequipppedAttachSocketName)
{
	AttachToComponentEquipped = InAttachToComponentEquipped;
	AttachToComponentUnequipped = InAttachToComponentUnequipped;
	EquippedAttachSocketName = InEquipppedAttachSocketName;
	UnequippedAttachSocketName = InUnequipppedAttachSocketName;
	Equipment = InEquipment;

	if (!Check())
	{
		UE_LOG(LogEquipmentSystem, Warning, TEXT("UEquipActorSwitcher::Init: Check failed. Disabling component."));
		SetActive(false);
		return;
	}

	Equipment->OnAnyItemEquipNotify.AddDynamic(this, &UEquipActorSwitcher::OnAnyItemEquipped);
	Equipment->OnAnyItemUnequipNotify.AddDynamic(this, &UEquipActorSwitcher::OnAnyItemUnequipped);
	Equipment->OnAnyItemEquipSlotChanged.AddDynamic(this, &UEquipActorSwitcher::OnAnyItemEquipSlotChanged);
	Equipment->OnItemsSwitchedSlots.AddDynamic(this, &UEquipActorSwitcher::OnItemsSwitchedSlots);
	Equipment->OnAnyItemSwapWithInventoryItem.AddDynamic(this, &UEquipActorSwitcher::OnAnyItemSwapWithInventoryItem);
	Equipment->OnEquipmentCleared.AddDynamic(this, &UEquipActorSwitcher::OnEquipmentCleared);
}

AActor* UEquipActorSwitcher::GetHeldActor() const
{
	return HeldActor;
}

bool UEquipActorSwitcher::IsHoldingActor() const
{
	return IsValid(HeldActor);
}

void UEquipActorSwitcher::HoldNextActor()
{
	if (!Check())
		return;

	if (CurrentActors.IsEmpty())
		return;

	const int32 NextSlotIndex = FindNextAvailableSlotIndex();
	HoldActorAtSlot(NextSlotIndex);
}

void UEquipActorSwitcher::HoldPreviousActor()
{
	if (!Check())
		return;

	if (CurrentActors.IsEmpty())
		return;

	const int32 PreviousSlotIndex = FindPreviousAvailableSlotIndex();
	HoldActorAtSlot(PreviousSlotIndex);
}

void UEquipActorSwitcher::HoldActorAtSlot(const int32 SlotIndex)
{
	if (!Check() || !CurrentActors.Contains(SlotIndex))
		return;

	AActor* Actor = CurrentActors.FindRef(SlotIndex);
	if (!IsValid(Actor) || Actor == HeldActor)
		return;

	WithdrawActor(HeldActor);
	HoldActor(Actor);
	CurrentHeldActorIndex = SlotIndex;
}

void UEquipActorSwitcher::WithdrawActorAtSlot(const int32 SlotIndex)
{
	if (!Check() || !CurrentActors.Contains(SlotIndex))
		return;

	AActor* Actor = CurrentActors.FindRef(SlotIndex);
	WithdrawActor(Actor);
	CurrentHeldActorIndex = -1;
}

TMap<int32, AActor*> UEquipActorSwitcher::GetCurrentActors() const
{
	return CurrentActors;
}

int32 UEquipActorSwitcher::GetCurrentHeldActorIndex() const
{
	return CurrentHeldActorIndex;
}

int32 UEquipActorSwitcher::FindNextAvailableSlotIndex() const
{
	if (CurrentActors.Num() <= 1)
		return GetHeldActorSlotIndex();

	const int32 Max = GetMaxSlotIndex();
	const int32 Min = GetMinSlotIndex();

	for (int32 i = GetHeldActorSlotIndex() + 1; i <= Max; ++i)
	{
		if (CurrentActors.Contains(i))
			return i;
	}

	return Min;
}

int32 UEquipActorSwitcher::FindPreviousAvailableSlotIndex() const
{
	if (CurrentActors.Num() <= 1)
		return GetHeldActorSlotIndex();

	const int32 Max = GetMaxSlotIndex();
	const int32 Min = GetMinSlotIndex();

	for (int32 i = GetHeldActorSlotIndex() - 1; i >= Min; --i)
	{
		if (CurrentActors.Contains(i))
			return i;
	}

	return Max;
}

void UEquipActorSwitcher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!IsValid(Equipment))
		return;

	Equipment->OnAnyItemEquipNotify.RemoveDynamic(this, &UEquipActorSwitcher::OnAnyItemEquipped);
	Equipment->OnAnyItemUnequipNotify.RemoveDynamic(this, &UEquipActorSwitcher::OnAnyItemUnequipped);
	Equipment->OnAnyItemEquipSlotChanged.RemoveDynamic(this, &UEquipActorSwitcher::OnAnyItemEquipSlotChanged);
	Equipment->OnItemsSwitchedSlots.RemoveDynamic(this, &UEquipActorSwitcher::OnItemsSwitchedSlots);
	Equipment->OnAnyItemSwapWithInventoryItem.RemoveDynamic(this, &UEquipActorSwitcher::OnAnyItemSwapWithInventoryItem);
}

void UEquipActorSwitcher::OnHoldActor_Implementation(AActor* Actor) const
{
}

void UEquipActorSwitcher::OnActorDisposed_Implementation(AActor* Actor) const
{
}

void UEquipActorSwitcher::OnWithdrawActor_Implementation(AActor* Actor) const
{
}

AActor* UEquipActorSwitcher::CreateActorToHold_Implementation(UItemBase* Item) const
{
	return nullptr;
}

void UEquipActorSwitcher::DisposeActor_Implementation(AActor* Actor)
{
	Actor->Destroy();
}

bool UEquipActorSwitcher::CanAddActor_Implementation(const UItemBase* Item) const
{
	return true;
}

void UEquipActorSwitcher::AddEquipActor(UItemBase* Item, const int32 SlotIndex)
{
	if (!IsValid(Item))
		return;
	
	if (!CanAddActor(Item))
		return;

	AActor* ActorToHold = CreateActorToHold(Item);
	if (!IsValid(ActorToHold))
	{
		UE_LOG(LogEquipmentSystem, Warning, TEXT("UEquipActorSwitcher::AddEquipActor: ActorToHold is not valid."));
		return;
	}

	CurrentActors.Add(SlotIndex, ActorToHold);
	WithdrawActorAtSlot(SlotIndex);

	if (!IsHoldingActor())
		HoldActorAtSlot(SlotIndex);
}

void UEquipActorSwitcher::HoldFirstActor()
{
	const int32 MinSlotIndex = GetMinSlotIndex();
	HoldActorAtSlot(MinSlotIndex);
}

void UEquipActorSwitcher::HoldLastActor()
{
	const int32 MaxSlotIndex = GetMaxSlotIndex();
	HoldActorAtSlot(MaxSlotIndex);
}

void UEquipActorSwitcher::RemoveEquipActor(const int32 SlotIndex, const bool bFindNewActorToHold)
{
	if (!CurrentActors.Contains(SlotIndex))
		return;

	AActor* Actor = CurrentActors.FindRef(SlotIndex);
	CurrentActors.Remove(SlotIndex);

	if (IsValid(Actor)) // Dispose the actor first
	{
		OnActorDisposed(Actor);
		OnActorDisposedEvent.Broadcast(Actor);
		OnEquipActorsChangedEvent.Broadcast();
		DisposeActor(Actor);
	}

	if (Actor == HeldActor)
	{
		HeldActor = nullptr;
		CurrentHeldActorIndex = -1;

		if (bFindNewActorToHold)
			HoldLastActor(); // In case we are holding the actor we want to remove
	}
}

int32 UEquipActorSwitcher::GetMaxSlotIndex() const
{
	int32 MaxSlotIndex = 0;
	for (auto It = CurrentActors.CreateConstIterator(); It; ++It)
		MaxSlotIndex = FMath::Max(MaxSlotIndex, It.Key());

	return MaxSlotIndex;
}

int32 UEquipActorSwitcher::GetMinSlotIndex() const
{
	int32 MinSlotIndex = 0;
	for (auto It = CurrentActors.CreateConstIterator(); It; ++It)
		MinSlotIndex = FMath::Min(MinSlotIndex, It.Key());

	return MinSlotIndex;
}

int32 UEquipActorSwitcher::GetHeldActorSlotIndex() const
{
	if (!IsHoldingActor())
		return -1;

	for (auto It = CurrentActors.CreateConstIterator(); It; ++It)
	{
		if (It.Value() == HeldActor)
			return It.Key();
	}

	return -1;
}

void UEquipActorSwitcher::HoldActor(AActor* Actor)
{
	if (!IsValid(Actor))
		return;

	HeldActor = Actor;
	HeldActor->SetActorHiddenInGame(false);
	HeldActor->AttachToComponent(AttachToComponentEquipped, FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquippedAttachSocketName);

	OnHoldActor(Actor);
	OnHoldActorEvent.Broadcast(Actor);
	OnEquipActorsChangedEvent.Broadcast();
}

void UEquipActorSwitcher::WithdrawActor(AActor* Actor) const
{
	if (!IsValid(Actor))
		return;

	if (IsValid(AttachToComponentUnequipped))
		Actor->AttachToComponent(AttachToComponentUnequipped, FAttachmentTransformRules::SnapToTargetNotIncludingScale, UnequippedAttachSocketName);
	else
		Actor->SetActorHiddenInGame(true);

	OnWithdrawActor(Actor);
	OnWithdrawActorEvent.Broadcast(Actor);
	OnEquipActorsChangedEvent.Broadcast();
}

void UEquipActorSwitcher::ChangeEquipActorSlot(const int32 OldSlotIndex, const int32 NewSlotIndex)
{
	if (!CurrentActors.Contains(OldSlotIndex))
	{
		UE_LOG(LogEquipmentSystem, Warning, TEXT("EquipActorSwitcher::ChangeEquipActorSlot: OldSlotIndex %d is not valid."), OldSlotIndex);
		return;
	}

	CurrentActors.Add(NewSlotIndex, CurrentActors[OldSlotIndex]);
	CurrentActors.Remove(OldSlotIndex);
}

void UEquipActorSwitcher::SwitchEquipActorsSlots(const int OldSlotIndexItemA, const int NewSlotIndexItemA, const int OldSlotIndexItemB, const int NewSlotIndexItemB)
{
	if (!CurrentActors.Contains(OldSlotIndexItemA) || !CurrentActors.Contains(OldSlotIndexItemB))
	{
		UE_LOG(LogEquipmentSystem, Warning, TEXT("EquipActorSwitcher::SwitchEquipActorsSlots: OldSlotIndex %d or %d is not valid."), OldSlotIndexItemA, OldSlotIndexItemB);
		return;
	}

	AActor* ActorInSlotA = CurrentActors[OldSlotIndexItemB];
	AActor* ActorInSlotB = CurrentActors[OldSlotIndexItemA];

	CurrentActors.Remove(OldSlotIndexItemB);
	CurrentActors.Remove(OldSlotIndexItemA);

	CurrentActors.Add(OldSlotIndexItemB, ActorInSlotB);
	CurrentActors.Add(OldSlotIndexItemA, ActorInSlotA);
}

bool UEquipActorSwitcher::IsTargetEquipSlotKeyValid(const UEquipSlotKey* EquipSlotKey) const
{
	return IsValid(EquipSlotKey) && EquipSlotKey == TargetEquipSlotKey;
}

bool UEquipActorSwitcher::Check() const
{
	return IsValid(Equipment) && IsValid(TargetEquipSlotKey) && IsValid(AttachToComponentEquipped);
}

void UEquipActorSwitcher::OnAnyItemEquipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* Item)
{
	if (!IsTargetEquipSlotKeyValid(EquipSlotKey))
		return;
	
	AddEquipActor(Item, SlotIndex);
}

void UEquipActorSwitcher::OnAnyItemUnequipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* Item)
{
	if (!IsTargetEquipSlotKeyValid(EquipSlotKey))
		return;
	
	RemoveEquipActor(SlotIndex);
}

void UEquipActorSwitcher::OnAnyItemEquipSlotChanged(UEquipSlotKey* EquipSlotKey, UItemBase* Item, int32 NewSlotIndex, int32 OldSlotIndex)
{
	if (!IsTargetEquipSlotKeyValid(EquipSlotKey))
		return;
	
	ChangeEquipActorSlot(OldSlotIndex, NewSlotIndex);
}

void UEquipActorSwitcher::OnItemsSwitchedSlots(UEquipSlotKey* EquipSlotKey, UItemBase* ItemA, UItemBase* ItemB, int OldSlotIndexItemA, int OldSlotIndexItemB, int NewSlotIndexItemA, int NewSlotIndexItemB)
{
	if (!IsTargetEquipSlotKeyValid(EquipSlotKey))
		return;
	
	SwitchEquipActorsSlots(OldSlotIndexItemA, NewSlotIndexItemA, OldSlotIndexItemB, NewSlotIndexItemB);
}

void UEquipActorSwitcher::OnAnyItemSwapWithInventoryItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item, UItemBase* InventoryItem, int32 SlotIndex)
{
	if (!IsTargetEquipSlotKeyValid(EquipSlotKey))
		return;

	RemoveEquipActor(SlotIndex, false);
	AddEquipActor(Item, SlotIndex);
}

void UEquipActorSwitcher::OnEquipmentCleared()
{
	for (const auto& SlotActorPair : CurrentActors)
	{
		AActor* Actor = SlotActorPair.Value;
		if (IsValid(Actor))
		{
			OnActorDisposed(Actor);
			OnActorDisposedEvent.Broadcast(Actor);
			DisposeActor(Actor);
		}
	}

	CurrentActors.Empty();
	HeldActor = nullptr;
	CurrentHeldActorIndex = -1;
	OnEquipActorsChangedEvent.Broadcast();
}
