// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/LoadingScreenUtility.h"

TWeakObjectPtr<ULoadingScreenSubsystem> ULoadingScreenUtility::LoadingScreenSubsystem = nullptr;

void ULoadingScreenUtility::Init(ULoadingScreenSubsystem* Subsystem)
{
	LoadingScreenSubsystem = Subsystem;
}

void ULoadingScreenUtility::SetupLoadingScreen(const ULoadingScreenData* LoadingScreenData, const bool bAutoResetToDefault)
{
	if (!Check())
		return;

	LoadingScreenSubsystem.Get()->SetupLoadingScreen(LoadingScreenData, bAutoResetToDefault);
}

void ULoadingScreenUtility::ResetToDefaultLoadingScreen()
{
	if (!Check())
		return;

	LoadingScreenSubsystem.Get()->ResetToDefaultLoadingScreen();
}

void ULoadingScreenUtility::ClearLoadingScreen(const bool bAutoResetToDefault)
{
	if (!Check())
		return;

	LoadingScreenSubsystem.Get()->ClearLoadingScreen(bAutoResetToDefault);
}

bool ULoadingScreenUtility::Check()
{
	return LoadingScreenSubsystem.IsValid();
}
