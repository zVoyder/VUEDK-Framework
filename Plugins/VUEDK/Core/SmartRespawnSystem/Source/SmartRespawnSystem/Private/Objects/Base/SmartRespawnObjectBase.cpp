// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/Base/SmartRespawnObjectBase.h"
#include "SmartRespawnSubsystem.h"

void USmartRespawnObjectBase::Init(USmartRespawnSubsystem* InSmartRespawnSubsystem)
{
	SmartRespawnSubsystem = InSmartRespawnSubsystem;
	OnInit();
}

#if WITH_EDITOR
bool USmartRespawnObjectBase::ImplementsGetWorld() const
{
	return true;
}
#endif

UWorld* USmartRespawnObjectBase::GetWorld() const
{
	return IsValid(SmartRespawnSubsystem) ? SmartRespawnSubsystem->GetWorld() : nullptr;
}

void USmartRespawnObjectBase::OnInit_Implementation()
{
}
