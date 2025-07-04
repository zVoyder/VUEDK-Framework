// Copyright VUEDK, Inc. All Rights Reserved.

#include "CrosshairSystemSettings.h"

UCrosshairSystemSettings::UCrosshairSystemSettings(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

TSubclassOf<UUserWidget> UCrosshairSystemSettings::GetDefaultCrosshairClass() const
{
	return DefaultCrosshairWidget.LoadSynchronous();
}
