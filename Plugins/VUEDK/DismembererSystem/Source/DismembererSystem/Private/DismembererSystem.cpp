// Copyright VUEDK, Inc. All Rights Reserved.

#include "DismembererSystem.h"
#include "DismembererSystemSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FDismembererSystemModule"

void FDismembererSystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Dismemberer System Settings",
			LOCTEXT("RuntimeSettingsName", "Dismemberer System Settings"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Dismemberer System Settings Plugin"),
			GetMutableDefault<UDismembererSystemSettings>()
		);
	}
}

void FDismembererSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "Dismemberer System Settings");
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDismembererSystemModule, DismembererSystem)
