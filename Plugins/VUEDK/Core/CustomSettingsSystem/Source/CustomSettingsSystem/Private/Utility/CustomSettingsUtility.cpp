// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/CustomSettingsUtility.h"
#include "CustomSettingsSystem.h"

UCustomGameUserSettings* UCustomSettingsUtility::GetCustomUserSettings()
{
	if (!IsValid(GEngine))
	{
		UE_LOG(LogCustomSettingsSystem, Warning, TEXT("UCustomUserSettingsUtility::GetCustomUserSettings: GEngine is null."));
		return nullptr;
	}

	return Cast<UCustomGameUserSettings>(GEngine->GetGameUserSettings());
}
