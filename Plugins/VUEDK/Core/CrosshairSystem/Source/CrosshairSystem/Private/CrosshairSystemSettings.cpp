// Copyright VUEDK, Inc. All Rights Reserved.

#include "CrosshairSystemSettings.h"

UCrosshairSystemSettings::UCrosshairSystemSettings(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

TSubclassOf<UCrosshairWidget> UCrosshairSystemSettings::GetGeneralCrosshairClass() const
{
	return GeneralCrosshairWidget.LoadSynchronous();
}
