// Copyright VUEDK, Inc. All Rights Reserved.

#include "CustomOptionObject.h"

void UCustomOptionObject::Init(UWorld* InWorld)
{
	CachedWorld = InWorld;
	OnInit();
}

void UCustomOptionObject::OnInit_Implementation()
{
}

void UCustomOptionObject::OnCustomSettingsApplied_Implementation(const float Value)
{
}

void UCustomOptionObject::OnCustomOptionChanged_Implementation(const float Value)
{
}

#if WITH_EDITOR
bool UCustomOptionObject::ImplementsGetWorld() const
{
	return true;
}
#endif

UWorld* UCustomOptionObject::GetWorld() const
{
	return CachedWorld;
}
