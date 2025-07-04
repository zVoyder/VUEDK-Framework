// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/CustomGameUserSettingsUtility.h"
#include "CustomSettingsSystem.h"

UCustomGameUserSettings* UCustomUserSettingsUtility::UserSettings = nullptr;

UCustomGameUserSettings* UCustomUserSettingsUtility::GetCustomUserSettings()
{
	if (!Check())
	{
		if (!IsValid(GEngine))
		{
			UE_LOG(LogCustomSettingsSystem, Error, TEXT("UCustomUserSettingsUtility::GetCustomUserSettings: GEngine is null."));
			return nullptr;
		}
		
		UserSettings = Cast<UCustomGameUserSettings>(GEngine->GetGameUserSettings());
	}
	
	return UserSettings;
}

bool UCustomUserSettingsUtility::Check()
{
	return IsValid(UserSettings);
}
