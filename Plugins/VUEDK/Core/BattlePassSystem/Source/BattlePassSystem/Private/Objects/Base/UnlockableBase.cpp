// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/Base/UnlockableBase.h"

void UUnlockableBase::Unlock()
{
	SetLockedState(false, true);
}

void UUnlockableBase::Lock()
{
	SetLockedState(true, true);
}

void UUnlockableBase::SetLockedState(const bool bLocked, const bool bNotify)
{
	if (bIsLocked == bLocked)
		return;
	
	bIsLocked = bLocked;

	if (bNotify)
		NotifyState();
}

bool UUnlockableBase::IsUnlocked() const
{
	return !IsLocked();
}

bool UUnlockableBase::IsLocked() const
{
	return bIsLocked;
}

bool UUnlockableBase::CanChangeState() const
{
	return true;
}

void UUnlockableBase::NotifyState() const
{
	OnUnlockableStateChanged.Broadcast(IsLocked());
}
