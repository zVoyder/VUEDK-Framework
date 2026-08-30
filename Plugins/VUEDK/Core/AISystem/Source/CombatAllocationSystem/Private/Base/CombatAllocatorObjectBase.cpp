// Copyright VUEDK, Inc. All Rights Reserved.

#include "Base/CombatAllocatorObjectBase.h"
#include "CombatSlotsProviders/Base/CombatSlotsProviderBase.h"

void UCombatAllocatorObjectBase::Init(UCombatAllocator* InCombatAllocator, const FGameplayTag InProviderTag)
{
	CombatAllocator = InCombatAllocator;
	ProviderTag = InProviderTag;
	OnInit();
	ReceiveOnInit();
}

AActor* UCombatAllocatorObjectBase::GetOwner() const
{
	return IsValid(CombatAllocator) ? CombatAllocator->GetOwner() : nullptr;
}

#if WITH_EDITOR
bool UCombatAllocatorObjectBase::ImplementsGetWorld() const
{
	return UObject::ImplementsGetWorld();
}
#endif

UWorld* UCombatAllocatorObjectBase::GetWorld() const
{
	return IsValid(CombatAllocator) ? CombatAllocator->GetWorld() : nullptr;
}

const UCombatSlotsProviderBase* UCombatAllocatorObjectBase::GetCombatSlotsProvider() const
{
	if (IsValid(CombatAllocator))
	{
		if (const UCombatSlotsProviderBase* const* ProviderPtr = CombatAllocator->SlotsProviders.Find(ProviderTag))
		{
			if (IsValid(*ProviderPtr))
				return *ProviderPtr;
		}
	}

	return nullptr;
}

void UCombatAllocatorObjectBase::OnInit()
{
}
