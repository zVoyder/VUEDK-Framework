// Copyright VUEDK, Inc. All Rights Reserved.

#include "EquipmentSystem/EquipPrecondition.h"

bool UEquipPrecondition::IsPreconditionMet_Implementation(UItemBase* Item, class UEquipment* Equipment) const
{
	return true; // Default implementation, can be overridden in derived classes
}

UWorld* UEquipPrecondition::GetWorld() const
{
	return GEngine->GetWorld();
}

#if WITH_EDITOR
bool UEquipPrecondition::ImplementsGetWorld() const
{
	return true;
}
#endif
