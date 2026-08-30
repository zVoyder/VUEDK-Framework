// Copyright VUEDK, Inc. All Rights Reserved.

#include "BetterUISubsystem.h"
#include "Framework/Application/NavigationConfig.h"

void UBetterUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Settings = GetMutableDefault<UBetterUISettings>();
	SetSlateNavigationConfig();
}

void UBetterUISubsystem::SetSlateNavigationConfig() const
{
	if (!IsValid(Settings))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBetterUISubsystem::SetSlateNavigationConfig: Settings is not valid."));
		return;
	}
	
	FNavigationConfig& NavigationConfig = *FSlateApplication::Get().GetNavigationConfig();
	NavigationConfig.bTabNavigation = Settings->bTabNavigation;
	NavigationConfig.bAnalogNavigation = Settings->bAnalogNavigation;
	NavigationConfig.bKeyNavigation = Settings->bKeyNavigation;
}

