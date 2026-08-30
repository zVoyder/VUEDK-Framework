// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatAllocator.h"
#include "CombatSlotsProviders/Base/CombatSlotsProviderBase.h"

UCombatAllocator::UCombatAllocator()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatAllocator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickProviders(DeltaTime);
}

bool UCombatAllocator::TryGetSlot(const FGameplayTag ProviderTag, const AActor* Requester, FCombatSlot& OutSlot)
{
	if (const UCombatSlotsProviderBase* const* ProviderPtr = SlotsProviders.Find(ProviderTag))
	{
		if (IsValid(*ProviderPtr))
			return (*ProviderPtr)->TryGetSlot(Requester, OutSlot);
	}

	return false;
}

bool UCombatAllocator::TryGetSlotAt(const FGameplayTag ProviderTag, const int32 SlotIndex, FCombatSlot& OutSlot)
{
	if (const UCombatSlotsProviderBase* const* ProviderPtr = SlotsProviders.Find(ProviderTag))
	{
		if (IsValid(*ProviderPtr))
			return (*ProviderPtr)->TryGetSlotAt(SlotIndex, OutSlot);
	}

	return false;
}

bool UCombatAllocator::ClaimSlot(const FGameplayTag ProviderTag, AActor* Requester, int32& OutSlotIndex)
{
	if (UCombatSlotsProviderBase** ProviderPtr = SlotsProviders.Find(ProviderTag))
	{
		if (IsValid(*ProviderPtr))
			return (*ProviderPtr)->ClaimSlot(Requester, OutSlotIndex);
	}

	return false;
}

bool UCombatAllocator::ClaimSlotAt(const FGameplayTag ProviderTag, AActor* Requester, const int32 SlotIndex)
{
	if (UCombatSlotsProviderBase** ProviderPtr = SlotsProviders.Find(ProviderTag))
	{
		if (IsValid(*ProviderPtr))
			return (*ProviderPtr)->ClaimSlotAt(Requester, SlotIndex);
	}

	return false;
}

bool UCombatAllocator::ReleaseSlot(const FGameplayTag ProviderTag, const AActor* Requester)
{
	if (UCombatSlotsProviderBase** ProviderPtr = SlotsProviders.Find(ProviderTag))
	{
		if (IsValid(*ProviderPtr))
			return (*ProviderPtr)->ReleaseSlot(Requester);
	}

	return false;
}

bool UCombatAllocator::ReleaseSlotAt(const FGameplayTag ProviderTag, const int32 SlotIndex)
{
	if (UCombatSlotsProviderBase** ProviderPtr = SlotsProviders.Find(ProviderTag))
	{
		if (IsValid(*ProviderPtr))
			return (*ProviderPtr)->ReleaseSlotAt(SlotIndex);
	}

	return false;
}

void UCombatAllocator::ReleaseAllSlots(const FGameplayTag ProviderTag)
{
	if (UCombatSlotsProviderBase** ProviderPtr = SlotsProviders.Find(ProviderTag))
	{
		if (IsValid(*ProviderPtr))
			(*ProviderPtr)->ReleaseAllSlots();
	}
}

void UCombatAllocator::ReleaseSlotFromAllProviders(const AActor* Requester)
{
	for (const auto& Provider : SlotsProviders)
	{
		if (IsValid(Provider.Value))
			Provider.Value->ReleaseSlot(Requester);
	}
}

bool UCombatAllocator::HasSlot(const FGameplayTag ProviderTag, const AActor* Requester, int32& OutSlotIndex) const
{
	if (const UCombatSlotsProviderBase* const* ProviderPtr = SlotsProviders.Find(ProviderTag))
	{
		if (IsValid(*ProviderPtr))
			return (*ProviderPtr)->HasSlot(Requester, OutSlotIndex);
	}

	return false;
}

bool UCombatAllocator::HasAvailableSlots(const FGameplayTag ProviderTag) const
{
	if (const UCombatSlotsProviderBase* const* ProviderPtr = SlotsProviders.Find(ProviderTag))
	{
		if (IsValid(*ProviderPtr))
			return (*ProviderPtr)->HasAvailableSlots();
	}

	return false;
}

bool UCombatAllocator::FindUsingProviders(const AActor* Requester, TArray<FGameplayTag>& OutProviderTags) const
{
	for (const auto& Provider : SlotsProviders)
	{
		if (IsValid(Provider.Value))
		{
			int32 OutSlotIndex;
			if (Provider.Value->HasSlot(Requester, OutSlotIndex))
				OutProviderTags.Add(Provider.Key);
		}
	}

	return OutProviderTags.Num() > 0;
}

void UCombatAllocator::BeginPlay()
{
	Super::BeginPlay();
	InitProviders();
}

void UCombatAllocator::InitProviders()
{
	for (const auto& Prov : SlotsProviders)
	{
		if (IsValid(Prov.Value))
			Prov.Value->Init(this, Prov.Key);
	}
}

void UCombatAllocator::TickProviders(const float DeltaTime)
{
	for (const auto& Prov : SlotsProviders)
	{
		if (IsValid(Prov.Value))
			Prov.Value->Tick(DeltaTime);
	}
}
